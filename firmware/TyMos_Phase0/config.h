#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// I2C Configuration
#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22
#define I2C_CLOCK_SPEED 400000

// PCA9685 Configuration
#define PCA9685_ADDR_HOURS 0x40
#define PCA9685_ADDR_MINUTES 0x41
#define PCA9685_PWM_FREQ 50

// Servo Configuration
#define SERVO_MIN_PULSE_US 500
#define SERVO_MAX_PULSE_US 2500
#define SERVO_STAGGER_DELAY_MS 20
#define SERVO_IDLE_TIMEOUT_MS 500

// Motion Configuration - Speed Profiles
// All profiles use 5° steps with different delays
#define SPEED_STEP_DEGREES 5        // Degrees per step (same for all profiles)
#define SPEED_FAST_DELAY_MS 10      // Delay between steps - FAST (quick)
#define SPEED_NORMAL_DELAY_MS 50    // Delay between steps - NORMAL (standard)
#define SPEED_NIGHT_DELAY_MS 100    // Delay between steps - NIGHT (silent)

// Speed Profile Enum
enum SpeedProfile {
  SPEED_FAST,    // 5° every 10ms (fast)
  SPEED_NORMAL,  // 5° every 50ms (standard)
  SPEED_NIGHT    // 5° every 100ms (silent)
};

// Segment Angles (Default)
#define ANGLE_REST_STANDARD 165
#define ANGLE_ACTIVE_STANDARD 70
#define ANGLE_REST_INVERTED 5
#define ANGLE_ACTIVE_INVERTED 100
#define ANGLE_INTERMEDIATE_STANDARD 100
#define ANGLE_INTERMEDIATE_INVERTED 70

// Segment 7 Special Case collision
// Segment 7 is Standard* : Active 70, Rest 165 (Internal)

#endif // CONFIG_H
