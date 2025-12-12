#ifndef LOGITECH_G29_SHIFTER_H
#define LOGITECH_G29_SHIFTER_H

#include <Arduino.h>

class LogitechG29Shifter {
 public:
  enum Gear {
    NEUTRAL = 0,
    GEAR_1 = 1,
    GEAR_2 = 2,
    GEAR_3 = 3,
    GEAR_4 = 4,
    GEAR_5 = 5,
    GEAR_6 = 6,
    REVERSE = 8
  };

  LogitechG29Shifter(uint8_t xPin, uint8_t yPin, uint8_t buttonPin);

  void begin();
  Gear getGear();

 private:
  uint8_t _xPin;
  uint8_t _yPin;
  uint8_t _buttonPin;

  // Thresholds
  static const int XAXIS_LEFT_THRESH = 365;
  static const int XAXIS_RIGHT_THRESH = 580;
  static const int YAXIS_UP_THRESH = 700;
  static const int YAXIS_DOWN_THRESH = 300;
};

#endif // LOGITECH_G29_SHIFTER_H
