#ifndef MOTION_SEGMENT_MAP_H
#define MOTION_SEGMENT_MAP_H

#include "config.h"
#include <Arduino.h>

enum DigitPosition {
  DIGIT_DO = 0, // Tens Hours
  DIGIT_UO = 1, // Units Hours
  DIGIT_DM = 2, // Tens Minutes
  DIGIT_UM = 3  // Units Minutes
};

struct SegmentConfig {
  int active;
  int rest;
  int intermediate; // -1 if none
};

class MotionSegmentMap {
public:
  // Get board and channel for a specific segment (1-7) of a digit
  static void getChannel(DigitPosition digit, int segment, uint8_t &boardAddr,
                         uint8_t &channel);

  // Dedicated method for Separator
  static void getChannelSeparator(uint8_t &boardAddr, uint8_t &channel);

  // Get angles for a segment (1-7)
  // Returns active, rest, and intermediate angles
  static SegmentConfig getAngles(int segment);

  // Helper to get segments active for a number (0-9)
  // buffer must be size 7. Returns true/false for each segment 1-7.
  static void getSegmentsForDigit(int number, bool *buffer);
};

#endif // MOTION_SEGMENT_MAP_H
