#ifndef CORE_SETTINGS_MANAGER_H
#define CORE_SETTINGS_MANAGER_H

#include "config.h"
#include <Arduino.h>

class CoreSettingsManager {
public:
  CoreSettingsManager();

  // Speed management
  void setSpeed(SpeedProfile speed);
  SpeedProfile getSpeed();

  // Night mode helper
  bool isNightMode();
  void setNightMode(bool enabled);

private:
  SpeedProfile _currentSpeed;
};

// Global settings instance
extern CoreSettingsManager Settings;

#endif // CORE_SETTINGS_MANAGER_H
