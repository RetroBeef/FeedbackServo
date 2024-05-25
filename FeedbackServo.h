#pragma once
#include <ESP32Servo.h>

typedef struct {
  uint8_t minValue;
  uint8_t centerValue;
  uint8_t maxValue;
} servo_values_t;

typedef struct {
  uint16_t minValue;
  uint16_t centerValue;
  uint16_t maxValue;
} servo_feedback_values_t;

typedef struct {
  Servo* servo;
  uint8_t servoPin;
  uint8_t feedbackPin;
  servo_values_t servoValues;
  servo_feedback_values_t servoFeedbackValues;
} feedback_servo_t;

class FeedbackServo {
  protected:
    Servo servo;
    feedback_servo_t feedbackServo;
  public:
    FeedbackServo(const uint8_t servoPin, const uint8_t feedbackPin, const uint8_t servoMin, const uint8_t servoCenter, const uint8_t servoMax, const uint16_t feedbackMin, const uint16_t feedbackCenter, const uint16_t feedbackMax);
    virtual ~FeedbackServo();
    void write(const uint8_t val);
    void smartWrite(const uint8_t val);
    uint16_t getAnalogFeedback();
};
