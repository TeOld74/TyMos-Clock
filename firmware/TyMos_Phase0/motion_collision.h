#ifndef MOTION_COLLISION_H
#define MOTION_COLLISION_H

#include "motion_segment_map.h"
#include "motion_servo.h"
#include <Arduino.h>

class MotionCollision {
public:
  MotionCollision(MotionServo *servo);

  // Check if collision logic is needed (i.e. if segment 7 changes state)
  // Actually, we usually pass the full digit info to decide
  bool needsCollisionLogic(int fromNum, int toNum);

  // Execute the collision avoidance sequence (Blocking)
  // Applies the change from fromNum to toNum for the given digit
  // ONLY handles the "transition" logic. The surrounding segments (1,3,4,5)
  // are moved first or handled by this sequence.
  // Architecture says:
  // 1. Seg 1,3,4,5 -> Final
  // 2. Seg 2,6 -> Simaltaneous Inter/Rest
  // 3. Seg 7 -> Final
  // 4. Seg 2,6 -> Final (if Active)
  void executeSequence(DigitPosition digit, int fromNum, int toNum);

private:
  MotionServo *_servo;

  // Helper to move 2 and 6 simultaneously with speed profile
  void _move2and6(DigitPosition digit, int angle2_start, int angle2_end,
                  int angle6_start, int angle6_end, SpeedProfile speed);
};

#endif // MOTION_COLLISION_H
