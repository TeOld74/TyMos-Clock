#include "core_time_manager.h"
#include "hw_rtc.h"
#include "utils_logger.h"

// Istanza globale
TimeManager TimeMgr;

void TimeManager::begin() {
  // Wrapper per inizializzazione RTC
  if (!RTC.begin()) {
    Logger.error("TimeManager: RTC initialization failed");
  }
}

DateTime TimeManager::getCurrentTime() { return RTC.now(); }

void TimeManager::syncFromCompileTime() { RTC.syncWithCompileTime(); }
