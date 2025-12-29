#ifndef MOTION_ENGINE_H
#define MOTION_ENGINE_H

#include "motion_collision.h"
#include "motion_segment_map.h"
#include "motion_servo.h"
#include <Arduino.h>

class MotionEngine {
public:
  MotionEngine(MotionServo *servo, MotionCollision *collision);

  // Initial Reset Sequence (Test Iniziale)
  // Phase 1 (REST): DO->UO->SEP->DM->UM (1->7)
  // Phase 2 (ACTIVE): UM->DM->SEP->UO->DO (7->1)
  void resetSequence();

  // Update a digit from one number to another
  // Handles collision logic if needed, otherwise standard staggered update
  void updateDigit(DigitPosition getDigit, int fromNum, int toNum);

  // Control Separator
  void setSeparator(bool active);

  // Tick method to be called in loop (for idle management)
  void tick();

private:
  MotionServo *_servo;
  MotionCollision *_collision;

  // Helper to move all segments of a digit to a specific state (Active/Rest)
  // with strictly ordered staggering (1->7 or 7->1)
  void _setDigitState(DigitPosition digit, bool active, bool reverseOrder);

  // Helper for reset sequence: moves segments one by one with custom delay
  // Used for test sequence with 500ms between each segment movement
  void _setDigitStateSequential(DigitPosition digit, bool active,
                                bool reverseOrder, int delayMs);

  // Helper to set separator state
  void _setSeparatorState(bool active);
};

#endif // MOTION_ENGINE_H
