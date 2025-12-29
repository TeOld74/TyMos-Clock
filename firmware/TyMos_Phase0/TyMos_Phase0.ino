/**
 * TyMos Clock - Phase 0
 * Mechatronic Clock with 29 Servo Motors
 *
 * Author: Matteo Oldani
 * Repository: https://github.com/TeOld74/TyMos-Clock
 * License: MIT
 *
 * Phase 0 Features:
 * - Automatic time display with 7-segment mechanical display
 * - WiFi connectivity
 * - NTP time synchronization
 * - OTA (Over-The-Air) updates
 * - Real-time clock with DS3231 RTC module
 */

#include <Arduino.h>

// Include secrets file (WiFi credentials, OTA password, etc.)
#if __has_include("secrets.h")
  #include "secrets.h"
#else
  #error "secrets.h not found! Copy secrets.h.template to secrets.h and fill in your credentials. See README.md for instructions."
#endif

#include "config.h"
#include "core_display_manager.h"
#include "core_settings_manager.h"
#include "hw_pca9685.h"
#include "hw_rtc.h"
#include "hw_wifi.h"
#include "motion_collision.h"
#include "motion_engine.h"
#include "motion_segment_map.h"
#include "motion_servo.h"
#include "utils_logger.h"

// Global Objects
HwPCA9685 pwmDriver;
RTCDriver rtcDriver;
HwWiFi wifiManager;
MotionServo motionServo(&pwmDriver);
MotionCollision motionCollision(&motionServo);
MotionEngine motionEngine(&motionServo, &motionCollision);
CoreDisplayManager displayManager(&rtcDriver, &motionEngine);

void setup() {
  // 1. Initialize Logger
  Logger.begin();
  Logger.info("Booting TyMos v2.0...");

  // 2. Initialize Hardware
  // I2C Setup
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  Wire.setClock(I2C_CLOCK_SPEED);

  // PWM Driver (Hours & Minutes)
  pwmDriver.begin(PCA9685_ADDR_HOURS, PCA9685_ADDR_MINUTES, PCA9685_PWM_FREQ);

  // RTC
  if (!rtcDriver.begin()) {
    Logger.error("RTC Initialization Failed!");
  }
  Logger.info("Hardware Initialized. Current Temp: %.2f C",
              rtcDriver.getTemperature());

  // 3. WiFi, NTP and OTA
  // Try to connect and sync time from NTP
  if (!wifiManager.begin()) {
    // WiFi failed, fallback to compile time
    Logger.warning("Using compile time as fallback");
    rtcDriver.syncWithCompileTime();
  }

  // 4. Initial Reset Sequence
  motionEngine.resetSequence();

  // 5. Configure Speed Based on Time (Night Mode: 22:00 - 07:00)
  DateTime now = rtcDriver.now();
  if (now.hour() >= 22 || now.hour() < 7) {
    Settings.setNightMode(true);
    Logger.info("Night mode enabled (22:00-07:00)");
  } else {
    Settings.setSpeed(SPEED_NORMAL);
  }

  // 6. Start Display Manager
  displayManager.begin();

  Logger.info("Setup Complete. Entering Loop.");
}

// Track last hour for night mode transition
static int lastHourChecked = -1;

void checkNightMode() {
  DateTime now = rtcDriver.now();
  int currentHour = now.hour();

  // Only check when hour changes
  if (currentHour != lastHourChecked) {
    lastHourChecked = currentHour;

    bool shouldBeNight = (currentHour >= 22 || currentHour < 7);
    bool isCurrentlyNight = Settings.isNightMode();

    if (shouldBeNight && !isCurrentlyNight) {
      Settings.setNightMode(true);
      Logger.info("Auto-switching to NIGHT mode");
    } else if (!shouldBeNight && isCurrentlyNight) {
      Settings.setNightMode(false);
      Logger.info("Auto-switching to NORMAL mode");
    }
  }
}

void loop() {
  // 1. Handle OTA updates
  wifiManager.handleOTA();

  // 2. Check for periodic NTP sync (every 12 hours)
  wifiManager.checkPeriodicSync();

  // 3. Check for automatic night mode transition
  checkNightMode();

  // 4. Motion Engine Tick (Handle idle, etc)
  motionEngine.tick();

  // 5. Display Update
  displayManager.update();

  // Small delay to prevent CPU hogging
  delay(1);
}
