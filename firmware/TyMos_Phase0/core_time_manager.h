#ifndef CORE_TIME_MANAGER_HPP
#define CORE_TIME_MANAGER_HPP

#include <Arduino.h>
#include <RTClib.h>

// ============================================================================
// TIME MANAGER - Gestione sincronizzazione tempo
// ============================================================================

class TimeManager {
public:
  void begin();
  DateTime getCurrentTime();
  void syncFromCompileTime();

private:
  // Nessuno stato interno necessario in FASE 0
};

extern TimeManager TimeMgr;

#endif // CORE_TIME_MANAGER_HPP
