#include "motion_servo.h"
#include "utils_logger.h"

MotionServo::MotionServo(HwPCA9685 *pwmDriver) {
  _pwm = pwmDriver;
  // Initialize trackers
  for (int b = 0; b < 2; b++) {
    for (int c = 0; c < 16; c++) {
      _lastMoveTime[b][c] = 0;
      _isActive[b][c] = false;
    }
  }
}

int MotionServo::_getBoardIndex(uint8_t addr) {
  if (addr == PCA9685_ADDR_HOURS)
    return 0;
  if (addr == PCA9685_ADDR_MINUTES)
    return 1;
  return -1;
}

uint16_t MotionServo::angleToPulse(int angle) {
  // Clamp angle
  if (angle < 0)
    angle = 0;
  if (angle > 180)
    angle = 180;

  // Map angle to microseconds
  long us = map(angle, 0, 180, SERVO_MIN_PULSE_US, SERVO_MAX_PULSE_US);

  // Convert microseconds to 12-bit counts (0-4095)
  // 50Hz = 20000us period
  // counts = (us * 4096) / 20000
  // Simplified: (us * 4096) / 20000 = (us * 1024) / 5000 = (us * 512) / 2500
  // Let's use floating point for precision or just long math
  // 20000us / 4096 = 4.8828 us per bit
  return (uint16_t)((us * 4096L) / 20000L);
}

void MotionServo::setAngle(uint8_t boardAddr, uint8_t channel, int angle) {
  int bIdx = _getBoardIndex(boardAddr);
  if (bIdx < 0)
    return;

  uint16_t pulse = angleToPulse(angle);
  _pwm->setPWM(boardAddr, channel, 0, pulse);

  _lastMoveTime[bIdx][channel] = millis();
  _isActive[bIdx][channel] = true;
}

void MotionServo::moveToAngle(uint8_t boardAddr, uint8_t channel, int fromAngle,
                              int toAngle, SpeedProfile speed) {
  if (fromAngle == toAngle)
    return;

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

  // Gradual movement with 5° steps for all profiles
  int direction = (toAngle > fromAngle) ? 1 : -1;
  int currentAngle = fromAngle;

  while (currentAngle != toAngle) {
    // Calculate next step
    int nextAngle = currentAngle + (direction * SPEED_STEP_DEGREES);

    // Check if we've exceeded the target
    if ((direction > 0 && nextAngle > toAngle) ||
        (direction < 0 && nextAngle < toAngle)) {
      nextAngle = toAngle;
    }

    setAngle(boardAddr, channel, nextAngle);
    currentAngle = nextAngle;

    // Delay only if we haven't reached the target
    if (currentAngle != toAngle) {
      delay(stepDelay);
    }
  }
}

void MotionServo::detach(uint8_t boardAddr, uint8_t channel) {
  int bIdx = _getBoardIndex(boardAddr);
  if (bIdx < 0)
    return;

  _pwm->setPWM(boardAddr, channel, 0, 0); // Full off
  _isActive[bIdx][channel] = false;
}

void MotionServo::checkIdle() {
  uint32_t now = millis();
  for (int b = 0; b < 2; b++) {
    uint8_t addr = (b == 0) ? PCA9685_ADDR_HOURS : PCA9685_ADDR_MINUTES;
    for (int c = 0; c < 16; c++) {
      if (_isActive[b][c]) {
        if (now - _lastMoveTime[b][c] > SERVO_IDLE_TIMEOUT_MS) {
          detach(addr, c);
          // Logger.debug("Auto-idle: Board 0x%X Ch %d", addr, c);
          // Commented out to reduce log spam unless needed
        }
      }
    }
  }
}
