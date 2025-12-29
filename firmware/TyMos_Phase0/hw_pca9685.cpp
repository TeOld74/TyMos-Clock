#include "hw_pca9685.h"
#include "utils_logger.h"

HwPCA9685::HwPCA9685() {
  _pwmHours = NULL;
  _pwmMinutes = NULL;
}

void HwPCA9685::begin(uint8_t addrHours, uint8_t addrMinutes, uint8_t freq) {
  setupI2C();

  _pwmHours = new Adafruit_PWMServoDriver(addrHours);
  _pwmMinutes = new Adafruit_PWMServoDriver(addrMinutes);

  if (_pwmHours) {
    _pwmHours->begin();
    _pwmHours->setPWMFreq(freq);
    Logger.info("PCA9685 Hours (0x%X) initialized at %dHz", addrHours, freq);
  } else {
    Logger.error("Failed to allocate PCA9685 Hours object");
  }

  if (_pwmMinutes) {
    _pwmMinutes->begin();
    _pwmMinutes->setPWMFreq(freq);
    Logger.info("PCA9685 Minutes (0x%X) initialized at %dHz", addrMinutes,
                freq);
  } else {
    Logger.error("Failed to allocate PCA9685 Minutes object");
  }
}

void HwPCA9685::setupI2C() {
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  Wire.setClock(I2C_CLOCK_SPEED);
  Logger.info("I2C initialized at 400kHz");
}

Adafruit_PWMServoDriver *HwPCA9685::_getDriver(uint8_t boardAddress) {
  if (_pwmHours && boardAddress == PCA9685_ADDR_HOURS)
    return _pwmHours;
  if (_pwmMinutes && boardAddress == PCA9685_ADDR_MINUTES)
    return _pwmMinutes;
  return NULL;
}

void HwPCA9685::setPWM(uint8_t boardAddress, uint8_t channel, uint16_t on,
                       uint16_t off) {
  Adafruit_PWMServoDriver *driver = _getDriver(boardAddress);
  if (driver) {
    driver->setPWM(channel, on, off);
  } else {
    Logger.error("setPWM: Invalid board address 0x%X", boardAddress);
  }
}

void HwPCA9685::reset(uint8_t boardAddress) {
  Adafruit_PWMServoDriver *driver = _getDriver(boardAddress);
  if (driver) {
    driver->reset();
    Logger.info("Reset PCA9685 at 0x%X", boardAddress);
  }
}

bool HwPCA9685::isConnected(uint8_t boardAddress) {
  Wire.beginTransmission(boardAddress);
  return (Wire.endTransmission() == 0);
}
