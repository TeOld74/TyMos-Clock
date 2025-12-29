#ifndef CORE_DISPLAY_MANAGER_H
#define CORE_DISPLAY_MANAGER_H

#include "hw_rtc.h"
#include "motion_engine.h"
#include <Arduino.h>

class CoreDisplayManager {
public:
  CoreDisplayManager(RTCDriver *rtc, MotionEngine *engine);

  // Initial display setup (force update)
  void begin();

  // Check time and update display if needed
  void update();

  // Force specific time display
  void showTime(int hours, int minutes, bool forceUpdates = false);

private:
  RTCDriver *_rtc;
  MotionEngine *_engine;

  // Track currently displayed digits to minimize movements
  int _currentDO;
  int _currentUO;
  int _currentDM;
  int _currentUM;

  uint32_t _lastUpdateCheck;
};

#endif // CORE_DISPLAY_MANAGER_H
