#include "motion_segment_map.h"

void MotionSegmentMap::getChannel(DigitPosition digit, int segment,
                                  uint8_t &boardAddr, uint8_t &channel) {
  // Segments are 1-7. Map to 0-6.
  int segIdx = segment - 1;
  if (segIdx < 0 || segIdx > 6) {
    // Invalid segment, safe fallback?
    boardAddr = 0;
    channel = 0;
    return;
  }

  switch (digit) {
  case DIGIT_DO:
    boardAddr = PCA9685_ADDR_HOURS;
    channel = segIdx;
    break;
  case DIGIT_UO:
    boardAddr = PCA9685_ADDR_HOURS;
    channel = 8 + segIdx;
    break;
  case DIGIT_DM:
    boardAddr = PCA9685_ADDR_MINUTES;
    channel = segIdx;
    break;
  case DIGIT_UM:
    boardAddr = PCA9685_ADDR_MINUTES;
    channel = 8 + segIdx;
    break;
  }
}

void MotionSegmentMap::getChannelSeparator(uint8_t &boardAddr,
                                           uint8_t &channel) {
  boardAddr = PCA9685_ADDR_HOURS;
  channel = 15;
}

SegmentConfig MotionSegmentMap::getAngles(int segment) {
  SegmentConfig cfg;
  cfg.intermediate = -1; // Default none

  // Standard: Active 70, Rest 165
  // Inverted (3, 6): Active 100, Rest 5

  // Segment 1
  if (segment == 1) {
    cfg.active = ANGLE_ACTIVE_STANDARD;
    cfg.rest = ANGLE_REST_STANDARD;
    cfg.intermediate = -1;
  }
  // Segment 2
  else if (segment == 2) {
    cfg.active = ANGLE_ACTIVE_STANDARD;
    cfg.rest = ANGLE_REST_STANDARD;
    cfg.intermediate = ANGLE_INTERMEDIATE_STANDARD; // Collision inter
  }
  // Segment 3 (Inv)
  else if (segment == 3) {
    cfg.active = ANGLE_ACTIVE_INVERTED;
    cfg.rest = ANGLE_REST_INVERTED;
    cfg.intermediate = -1;
  }
  // Segment 4
  else if (segment == 4) {
    cfg.active = ANGLE_ACTIVE_STANDARD;
    cfg.rest = ANGLE_REST_STANDARD;
    cfg.intermediate = -1;
  }
  // Segment 5
  else if (segment == 5) {
    cfg.active = ANGLE_ACTIVE_STANDARD;
    cfg.rest = ANGLE_REST_STANDARD;
    cfg.intermediate = -1;
  }
  // Segment 6 (Inv)
  else if (segment == 6) {
    cfg.active = ANGLE_ACTIVE_INVERTED;
    cfg.rest = ANGLE_REST_INVERTED;
    cfg.intermediate = ANGLE_INTERMEDIATE_INVERTED; // Collision inter
  }
  // Segment 7
  else if (segment == 7) {
    cfg.active = ANGLE_ACTIVE_STANDARD;
    cfg.rest = ANGLE_REST_STANDARD;
    cfg.intermediate = -1;
  } else {
    // Fallback
    cfg.active = ANGLE_ACTIVE_STANDARD;
    cfg.rest = ANGLE_REST_STANDARD;
  }
  return cfg;
}

void MotionSegmentMap::getSegmentsForDigit(int number, bool *buffer) {
  // Reset buffer
  for (int i = 0; i < 7; i++)
    buffer[i] = false;

  // Buffer is 0-indexed (Seg 1 is index 0, Seg 7 is index 6)
  // Mapping from architecture.rtf section 4.5:
  // | Cifra | Segmenti Attivi      |
  // |-------|----------------------|
  // | 0     | 1, 2, 3, 4, 5, 6     |
  // | 1     | 5, 6                 |
  // | 2     | 1, 2, 4, 5, 7        |
  // | 3     | 1, 4, 5, 6, 7        |
  // | 4     | 3, 5, 6, 7           |
  // | 5     | 1, 3, 4, 6, 7        |
  // | 6     | 1, 2, 3, 4, 6, 7     |
  // | 7     | 4, 5, 6              |
  // | 8     | 1, 2, 3, 4, 5, 6, 7  |
  // | 9     | 1, 3, 4, 5, 6, 7     |

  switch (number) {
  case 0:
    // Segmenti: 1, 2, 3, 4, 5, 6
    buffer[0] = true; // Seg 1
    buffer[1] = true; // Seg 2
    buffer[2] = true; // Seg 3
    buffer[3] = true; // Seg 4
    buffer[4] = true; // Seg 5
    buffer[5] = true; // Seg 6
    break;
  case 1:
    // Segmenti: 5, 6
    buffer[4] = true; // Seg 5
    buffer[5] = true; // Seg 6
    break;
  case 2:
    // Segmenti: 1, 2, 4, 5, 7
    buffer[0] = true; // Seg 1
    buffer[1] = true; // Seg 2
    buffer[3] = true; // Seg 4
    buffer[4] = true; // Seg 5
    buffer[6] = true; // Seg 7
    break;
  case 3:
    // Segmenti: 1, 4, 5, 6, 7
    buffer[0] = true; // Seg 1
    buffer[3] = true; // Seg 4
    buffer[4] = true; // Seg 5
    buffer[5] = true; // Seg 6
    buffer[6] = true; // Seg 7
    break;
  case 4:
    // Segmenti: 3, 5, 6, 7
    buffer[2] = true; // Seg 3
    buffer[4] = true; // Seg 5
    buffer[5] = true; // Seg 6
    buffer[6] = true; // Seg 7
    break;
  case 5:
    // Segmenti: 1, 3, 4, 6, 7
    buffer[0] = true; // Seg 1
    buffer[2] = true; // Seg 3
    buffer[3] = true; // Seg 4
    buffer[5] = true; // Seg 6
    buffer[6] = true; // Seg 7
    break;
  case 6:
    // Segmenti: 1, 2, 3, 4, 6, 7
    buffer[0] = true; // Seg 1
    buffer[1] = true; // Seg 2
    buffer[2] = true; // Seg 3
    buffer[3] = true; // Seg 4
    buffer[5] = true; // Seg 6
    buffer[6] = true; // Seg 7
    break;
  case 7:
    // Segmenti: 4, 5, 6
    buffer[3] = true; // Seg 4
    buffer[4] = true; // Seg 5
    buffer[5] = true; // Seg 6
    break;
  case 8:
    // Segmenti: 1, 2, 3, 4, 5, 6, 7 (tutti)
    for (int i = 0; i < 7; i++)
      buffer[i] = true;
    break;
  case 9:
    // Segmenti: 1, 3, 4, 5, 6, 7
    buffer[0] = true; // Seg 1
    buffer[2] = true; // Seg 3
    buffer[3] = true; // Seg 4
    buffer[4] = true; // Seg 5
    buffer[5] = true; // Seg 6
    buffer[6] = true; // Seg 7
    break;
  }
}
