#include "motion_collision.h"
#include "core_settings_manager.h"
#include "motion_segment_map.h"
#include "utils_logger.h"

MotionCollision::MotionCollision(MotionServo *servo) { _servo = servo; }

bool MotionCollision::needsCollisionLogic(int fromNum, int toNum) {
  bool segsFrom[7];
  bool segsTo[7];
  MotionSegmentMap::getSegmentsForDigit(fromNum, segsFrom);
  MotionSegmentMap::getSegmentsForDigit(toNum, segsTo);

  // Check Segment 7 (Index 6)
  bool s7_from = segsFrom[6];
  bool s7_to = segsTo[6];

  return (s7_from != s7_to);
}

void MotionCollision::executeSequence(DigitPosition digit, int fromNum,
                                      int toNum) {
  bool segsFrom[7];
  bool segsTo[7];
  MotionSegmentMap::getSegmentsForDigit(fromNum, segsFrom);
  MotionSegmentMap::getSegmentsForDigit(toNum, segsTo);

  uint8_t board, ch;
  SpeedProfile speed = Settings.getSpeed();

  // STEP 1: Process segments 1, 3, 4, 5
  // Move them to final position
  int nonCollisionSegs[] = {1, 3, 4, 5};
  for (int i = 0; i < 4; i++) {
    int seg = nonCollisionSegs[i];
    int idx = seg - 1;

    SegmentConfig angles = MotionSegmentMap::getAngles(seg);
    int startAngle = segsFrom[idx] ? angles.active : angles.rest;
    int targetAngle = segsTo[idx] ? angles.active : angles.rest;

    if (segsFrom[idx] != segsTo[idx]) {
      MotionSegmentMap::getChannel(digit, seg, board, ch);
      _servo->moveToAngle(board, ch, startAngle, targetAngle, speed);
      delay(SERVO_STAGGER_DELAY_MS);
    }
  }

  // STEP 2: Handle 2 and 6 collision safety
  SegmentConfig cfg2 = MotionSegmentMap::getAngles(2);
  SegmentConfig cfg6 = MotionSegmentMap::getAngles(6);

  // If final is Active -> Go Intermediate
  // If final is Rest -> Go Rest
  int target2_step2 = segsTo[1] ? cfg2.intermediate : cfg2.rest;
  int target6_step2 = segsTo[5] ? cfg6.intermediate : cfg6.rest;

  // Current positions (from fromNum state)
  int start2 = segsFrom[1] ? cfg2.active : cfg2.rest;
  int start6 = segsFrom[5] ? cfg6.active : cfg6.rest;

  // Execute Move 2 & 6 Simultaneous
  _move2and6(digit, start2, target2_step2, start6, target6_step2, speed);

  // STEP 3: Move Segment 7 to final
  int idx7 = 6;
  SegmentConfig cfg7 = MotionSegmentMap::getAngles(7);
  int start7 = segsFrom[idx7] ? cfg7.active : cfg7.rest;
  int target7 = segsTo[idx7] ? cfg7.active : cfg7.rest;

  MotionSegmentMap::getChannel(digit, 7, board, ch);
  _servo->moveToAngle(board, ch, start7, target7, speed);

  // STEP 4: Move 2 and 6 to final Active (if they paused at Intermediate)
  int final2 = segsTo[1] ? cfg2.active : cfg2.rest;
  int final6 = segsTo[5] ? cfg6.active : cfg6.rest;

  if (target2_step2 != final2 || target6_step2 != final6) {
    _move2and6(digit, target2_step2, final2, target6_step2, final6, speed);
  }
}

void MotionCollision::_move2and6(DigitPosition digit, int start2, int end2,
                                 int start6, int end6, SpeedProfile speed) {
  uint8_t b2, c2, b6, c6;
  MotionSegmentMap::getChannel(digit, 2, b2, c2);
  MotionSegmentMap::getChannel(digit, 6, b6, c6);

  // Select delay based on speed profile
  int stepDelay;
  switch (speed) {
    case SPEED_FAST:
      stepDelay = SPEED_FAST_DELAY_MS;
      break;
    case SPEED_NORMAL:
      stepDelay = SPEED_NORMAL_DELAY_MS;
      break;
    case SPEED_NIGHT:
    default:
      stepDelay = SPEED_NIGHT_DELAY_MS;
      break;
  }

  // Simultaneous gradual movement, 5° at a time for all profiles
  int dir2 = (end2 > start2) ? 1 : (end2 < start2) ? -1 : 0;
  int dir6 = (end6 > start6) ? 1 : (end6 < start6) ? -1 : 0;

  int curr2 = start2;
  int curr6 = start6;

  while (curr2 != end2 || curr6 != end6) {
    // Step segment 2
    if (curr2 != end2) {
      int next2 = curr2 + (dir2 * SPEED_STEP_DEGREES);
      if ((dir2 > 0 && next2 > end2) || (dir2 < 0 && next2 < end2)) {
        next2 = end2;
      }
      curr2 = next2;
      _servo->setAngle(b2, c2, curr2);
    }

    // Step segment 6
    if (curr6 != end6) {
      int next6 = curr6 + (dir6 * SPEED_STEP_DEGREES);
      if ((dir6 > 0 && next6 > end6) || (dir6 < 0 && next6 < end6)) {
        next6 = end6;
      }
      curr6 = next6;
      _servo->setAngle(b6, c6, curr6);
    }

    // Delay if not finished
    if (curr2 != end2 || curr6 != end6) {
      delay(stepDelay);
    }
  }
}
