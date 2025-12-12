#ifndef THRUSTMASTER_BASE_H
#define THRUSTMASTER_BASE_H

#include <Arduino.h>

class ThrustmasterBase {
 public:
  enum class SequentialPosition : uint8_t {
    CENTER = 0x04,
    DOWN = 0x05,
    UP = 0x06
  };

  ThrustmasterBase();

  // Sets the shifting mode: true for H-pattern, false for Sequential.
  void setHMode(bool isHMode);

  // Switches to a specific gear in H-pattern mode.
  // gear: 1-6 for gears, 8 for Reverse, 0 for Neutral.
  void switchHGear(uint8_t gear);

  // Switches gear in Sequential mode.
  void switchSequentialGear(SequentialPosition pos);

  // Sends the current command packet to the wheelbase via I2C.
  void sendCommand();

 private:
  uint8_t _command[14];
};

#endif // THRUSTMASTER_BASE_H
