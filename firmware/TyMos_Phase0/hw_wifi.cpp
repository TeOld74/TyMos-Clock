#include "hw_wifi.h"
#include "hw_rtc.h"
#include "utils_logger.h"
#include <ArduinoOTA.h>
#include <WiFi.h>
#include <time.h>

// Include secrets file for WiFi credentials and OTA password
#if __has_include("secrets.h")
  #include "secrets.h"
#else
  #error "secrets.h not found! Copy secrets.h.template to secrets.h and fill in your credentials."
#endif

// OTA Configuration
const char *OTA_HOSTNAME = "TyMos-Clock";

// External RTC driver
extern RTCDriver rtcDriver;

HwWiFi::HwWiFi() {
  _connected = false;
  _lastNTPSync = 0;
}

bool HwWiFi::begin() {
  Logger.info("Connecting to WiFi: %s", WIFI_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.setHostname(OTA_HOSTNAME);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  // Wait for connection (timeout 15 seconds)
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    _connected = true;
    Logger.info("WiFi connected! IP: %s", WiFi.localIP().toString().c_str());

    // Configure NTP
    configTime(TIMEZONE_OFFSET, 0, NTP_SERVER1, NTP_SERVER2);
    Logger.info("NTP configured: %s, %s (Timezone offset: %d seconds)",
                NTP_SERVER1, NTP_SERVER2, TIMEZONE_OFFSET);

    // Initial NTP sync
    syncNTP();

    // Setup OTA
    ArduinoOTA.setHostname(OTA_HOSTNAME);
    ArduinoOTA.setPassword(OTA_PASSWORD);

    ArduinoOTA.onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH) {
        type = "sketch";
      } else {
        type = "filesystem";
      }
      Logger.info("OTA Start: %s", type.c_str());
    });

    ArduinoOTA.onEnd([]() { Logger.info("OTA End - Rebooting..."); });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      static int lastPercent = -1;
      int percent = (progress / (total / 100));
      if (percent != lastPercent && percent % 10 == 0) {
        Logger.info("OTA Progress: %u%%", percent);
        lastPercent = percent;
      }
    });

    ArduinoOTA.onError([](ota_error_t error) {
      const char *errorMsg;
      switch (error) {
      case OTA_AUTH_ERROR:
        errorMsg = "Auth Failed";
        break;
      case OTA_BEGIN_ERROR:
        errorMsg = "Begin Failed";
        break;
      case OTA_CONNECT_ERROR:
        errorMsg = "Connect Failed";
        break;
      case OTA_RECEIVE_ERROR:
        errorMsg = "Receive Failed";
        break;
      case OTA_END_ERROR:
        errorMsg = "End Failed";
        break;
      default:
        errorMsg = "Unknown";
        break;
      }
      Logger.error("OTA Error: %s", errorMsg);
    });

    ArduinoOTA.begin();
    Logger.info("OTA ready. Hostname: %s", OTA_HOSTNAME);

    return true;
  } else {
    _connected = false;
    Logger.warning("WiFi connection failed! Continuing offline...");
    return false;
  }
}

void HwWiFi::handleOTA() {
  if (_connected) {
    ArduinoOTA.handle();
  }
}

bool HwWiFi::syncNTP() {
  if (!_connected) {
    Logger.warning("Cannot sync NTP: WiFi not connected");
    return false;
  }

  Logger.info("Syncing time from NTP...");

  // Wait for NTP time (max 5 seconds)
  struct tm timeinfo;
  int attempts = 0;
  while (!getLocalTime(&timeinfo) && attempts < 10) {
    delay(500);
    attempts++;
  }

  if (attempts >= 10) {
    Logger.error("NTP sync failed: timeout");
    return false;
  }

  // Update RTC with NTP time
  DateTime ntpTime(timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday,
                   timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

  rtcDriver.setTime(ntpTime);

  Logger.info("NTP sync OK: %02d/%02d/%04d %02d:%02d:%02d", timeinfo.tm_mday,
              timeinfo.tm_mon + 1, timeinfo.tm_year + 1900, timeinfo.tm_hour,
              timeinfo.tm_min, timeinfo.tm_sec);

  _lastNTPSync = millis();
  return true;
}

void HwWiFi::checkPeriodicSync() {
  if (!_connected) {
    return;
  }

  // Check if 12 hours have passed since last sync
  if (millis() - _lastNTPSync >= NTP_SYNC_INTERVAL_MS) {
    Logger.info("Periodic NTP sync (12h interval)...");
    syncNTP();
  }
}

bool HwWiFi::isConnected() {
  _connected = (WiFi.status() == WL_CONNECTED);
  return _connected;
}

String HwWiFi::getIPAddress() {
  if (_connected) {
    return WiFi.localIP().toString();
  }
  return "Not connected";
}
