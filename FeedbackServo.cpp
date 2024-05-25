#include "FeedbackServo.h"

namespace {
bool inRange(uint16_t valIs, uint16_t valShould, uint16_t range) {
  return ((valIs >= valShould - range) && (valIs <= valShould + range));
}
bool timerInitialized = 0;
}

FeedbackServo::FeedbackServo(const uint8_t servoPin, const uint8_t feedbackPin, const uint8_t servoMin, const uint8_t servoCenter, const uint8_t servoMax, const uint16_t feedbackMin, const uint16_t feedbackCenter, const uint16_t feedbackMax) {
  feedbackServo = {new Servo(), servoPin, feedbackPin, {servoMin, servoCenter, servoMax}, {feedbackMin, feedbackCenter, feedbackMax}};
  if (!timerInitialized) {
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);
    timerInitialized = 1;
  }
  feedbackServo.servo->setPeriodHertz(50);
  feedbackServo.servo->attach(servoPin, 500, 2500);
}

FeedbackServo::~FeedbackServo() {
  if (feedbackServo.servo->attached()){
    feedbackServo.servo->detach();
  }
  if(feedbackServo.servo){
    delete feedbackServo.servo;
    feedbackServo.servo = 0;
  }
}

void FeedbackServo::smartWrite(const uint8_t val) {
  uint8_t clippedVal = val;
  if (val < feedbackServo.servoValues.minValue) {
    clippedVal = feedbackServo.servoValues.minValue;
  }
  if (val > feedbackServo.servoValues.maxValue) {
    clippedVal = feedbackServo.servoValues.maxValue;
  }
  if (!feedbackServo.servo->attached()) {
    feedbackServo.servo->attach(feedbackServo.servoPin);
  }
  feedbackServo.servo->write(clippedVal);
  uint16_t expectedFeedback = map(clippedVal, 0, 180, 790, 3320);
  uint16_t analogFeedback = analogRead(feedbackServo.feedbackPin);
  uint32_t startedMs = millis();
  bool timeout = false;
  while (!inRange(analogFeedback, expectedFeedback, 100) && !timeout) {
    analogFeedback = analogRead(feedbackServo.feedbackPin);
    timeout = millis() - startedMs > 1000;
  }
  if (timeout) {
    Serial.println("servo timed out; position not completely reliable");
  }
  if (feedbackServo.servo->attached())feedbackServo.servo->detach();
}

void FeedbackServo::write(const uint8_t val) {
  uint8_t clippedVal = val;
  if (val < feedbackServo.servoValues.minValue) {
    clippedVal = feedbackServo.servoValues.minValue;
  }
  if (val > feedbackServo.servoValues.maxValue) {
    clippedVal = feedbackServo.servoValues.maxValue;
  }
  if (!feedbackServo.servo->attached()) {
    feedbackServo.servo->attach(feedbackServo.servoPin);
  }
  feedbackServo.servo->write(clippedVal);
}

uint16_t FeedbackServo::getAnalogFeedback() {
  return analogRead(feedbackServo.feedbackPin);
}
