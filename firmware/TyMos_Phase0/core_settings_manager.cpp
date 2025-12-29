#include "core_settings_manager.h"
#include "utils_logger.h"

// Global instance
CoreSettingsManager Settings;

CoreSettingsManager::CoreSettingsManager() { _currentSpeed = SPEED_NORMAL; }

void CoreSettingsManager::setSpeed(SpeedProfile speed) {
  _currentSpeed = speed;
  const char *speedName;
  switch (speed) {
    case SPEED_FAST:
      speedName = "FAST";
      break;
    case SPEED_NORMAL:
      speedName = "NORMAL";
      break;
    case SPEED_NIGHT:
    default:
      speedName = "NIGHT";
      break;
  }
  Logger.info("Speed set to: %s", speedName);
}

SpeedProfile CoreSettingsManager::getSpeed() { return _currentSpeed; }

bool CoreSettingsManager::isNightMode() { return _currentSpeed == SPEED_NIGHT; }

void CoreSettingsManager::setNightMode(bool enabled) {
  setSpeed(enabled ? SPEED_NIGHT : SPEED_NORMAL);
}
