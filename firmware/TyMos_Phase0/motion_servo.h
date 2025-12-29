#ifndef MOTION_SERVO_H
#define MOTION_SERVO_H

#include "config.h"
#include "hw_pca9685.h"
#include <Arduino.h>

class MotionServo {
public:
  MotionServo(HwPCA9685 *pwmDriver);

  // Convert angle (0-180) to PWM pulse
  uint16_t angleToPulse(int angle);

  // Set servo angle immediately (updates idle timer)
  void setAngle(uint8_t boardAddr, uint8_t channel, int angle);

  // Move servo to target angle with speed profile
  // FAST: 5° steps with 10ms delay
  // NORMAL: 5° steps with 50ms delay
  // NIGHT: 5° steps with 100ms delay
  void moveToAngle(uint8_t boardAddr, uint8_t channel, int fromAngle,
                   int toAngle, SpeedProfile speed);

  // Detach servo (PWM 0)
  void detach(uint8_t boardAddr, uint8_t channel);

  // Check for idle servos and detach them if timeout expired
  void checkIdle();

private:
  HwPCA9685 *_pwm;
  uint32_t _lastMoveTime[2][16]; // [boardIndex][channel] 0=0x40, 1=0x41
  bool _isActive[2][16];

  int _getBoardIndex(uint8_t addr);
};

#endif // MOTION_SERVO_H
