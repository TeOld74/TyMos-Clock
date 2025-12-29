#include "core_display_manager.h"
#include "utils_logger.h"

CoreDisplayManager::CoreDisplayManager(RTCDriver *rtc, MotionEngine *engine) {
  _rtc = rtc;
  _engine = engine;
  _currentDO = -1;
  _currentUO = -1;
  _currentDM = -1;
  _currentUM = -1;
  _lastUpdateCheck = 0;
}

void CoreDisplayManager::begin() {
  Logger.info("Display Manager initializing...");

  // After resetSequence(), display shows 88:88 (all segments active)
  // Initialize current state to match physical display state
  _currentDO = 8;
  _currentUO = 8;
  _currentDM = 8;
  _currentUM = 8;

  // Now update from 88:88 to actual time
  DateTime now = _rtc->now();
  Logger.info("Ora corrente RTC: %02d:%02d", now.hour(), now.minute());
  showTime(now.hour(), now.minute(), true);

  // Separator is already active from resetSequence, but ensure it
  _engine->setSeparator(true);
}

void CoreDisplayManager::update() {
  // Check every second? Or 500ms?
  if (millis() - _lastUpdateCheck > 1000) {
    DateTime now = _rtc->now();
    showTime(now.hour(), now.minute());
    _lastUpdateCheck = millis();
  }
}

void CoreDisplayManager::showTime(int hours, int minutes, bool forceUpdates) {
  int nextDO = hours / 10;
  int nextUO = hours % 10;
  int nextDM = minutes / 10;
  int nextUM = minutes % 10;

  // Update Decimals Hours (only if changed or forced)
  if (forceUpdates || nextDO != _currentDO) {
    _engine->updateDigit(DIGIT_DO, _currentDO, nextDO);
    _currentDO = nextDO;
  }

  // Update Units Hours
  if (forceUpdates || nextUO != _currentUO) {
    _engine->updateDigit(DIGIT_UO, _currentUO, nextUO);
    _currentUO = nextUO;
  }

  // Update Decimals Minutes
  if (forceUpdates || nextDM != _currentDM) {
    _engine->updateDigit(DIGIT_DM, _currentDM, nextDM);
    _currentDM = nextDM;
  }

  // Update Units Minutes
  if (forceUpdates || nextUM != _currentUM) {
    _engine->updateDigit(DIGIT_UM, _currentUM, nextUM);
    _currentUM = nextUM;
  }
}
