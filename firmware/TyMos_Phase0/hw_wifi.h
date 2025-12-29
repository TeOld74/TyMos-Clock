#ifndef HW_WIFI_H
#define HW_WIFI_H

#include <Arduino.h>

class HwWiFi {
public:
  HwWiFi();

  // Initialize WiFi, NTP and OTA
  // Returns true if WiFi connected successfully
  bool begin();

  // Call in loop() to handle OTA updates
  void handleOTA();

  // Sync time from NTP server and update RTC
  // Returns true if sync successful
  bool syncNTP();

  // Check if it's time for periodic NTP sync (every 12 hours)
  void checkPeriodicSync();

  // Get connection status
  bool isConnected();

  // Get IP address as string
  String getIPAddress();

private:
  bool _connected;
  unsigned long _lastNTPSync;
  static const unsigned long NTP_SYNC_INTERVAL_MS = 12UL * 60UL * 60UL * 1000UL; // 12 hours
};

#endif // HW_WIFI_H
