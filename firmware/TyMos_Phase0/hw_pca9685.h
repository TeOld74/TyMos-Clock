#ifndef HW_PCA9685_H
#define HW_PCA9685_H

#include "config.h"
#include <Adafruit_PWMServoDriver.h>
#include <Arduino.h>
#include <Wire.h>

class HwPCA9685 {
public:
  HwPCA9685();
  void begin(uint8_t addrHours, uint8_t addrMinutes, uint8_t freq);
  void setupI2C();
  void setPWM(uint8_t boardAddress, uint8_t channel, uint16_t on, uint16_t off);
  void reset(uint8_t boardAddress);
  bool isConnected(uint8_t boardAddress);

private:
  Adafruit_PWMServoDriver *_pwmHours;
  Adafruit_PWMServoDriver *_pwmMinutes;

  // Internal helper to get the correct driver instance
  Adafruit_PWMServoDriver *_getDriver(uint8_t boardAddress);
};

#endif // HW_PCA9685_H
