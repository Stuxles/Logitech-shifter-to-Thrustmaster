#include "ThrustmasterBase.h"
#include <Wire.h>

ThrustmasterBase::ThrustmasterBase() {
  // Initialize command array with default values
  // Based on "commands seen from oscilloscope in ISRTV forum"
  _command[0] = 0x00; // Shifter mode: 0 - S, 0x80 - H
  _command[1] = 0x0C; // Unknown
  _command[2] = 0x01; // Unknown
  _command[3] = 0x00; // Gear in H-mode
  _command[4] = 0x00; // Gear in S-Mode
  _command[5] = 0x80; // Unknown
  _command[6] = 0x80; // Unknown
  _command[7] = 0x00; // Y coordinate
  _command[8] = 0x00; // X coordinate
  _command[9] = 0x00; // Unknown
  _command[10] = 0x00; // Unknown
  _command[11] = 0x00; // Unknown
  _command[12] = 0x00; // Unknown
  _command[13] = 0x00; // Unknown
}

void ThrustmasterBase::setHMode(bool isHMode) {
  if (isHMode) {
    _command[0] |= 0x80;
  } else {
    _command[0] &= ~0x80;
  }
}

void ThrustmasterBase::switchHGear(uint8_t gear) {
  // Logic from original code:
  // gear 1 -> 0x80 >> 7 = 0x01
  // gear 2 -> 0x80 >> 6 = 0x02
  // ...
  // gear 6 -> 0x80 >> 2 = 0x20
  // gear 8 (Reverse) -> 0x80 >> 0 = 0x80
  // gear 0 (Neutral) -> 0x80 >> 8 = 0x00

  if (gear > 8) return; // Basic validation

  _command[3] = (0x80 >> (8 - gear));
}

void ThrustmasterBase::switchSequentialGear(SequentialPosition pos) {
  _command[4] = static_cast<uint8_t>(pos);
}

void ThrustmasterBase::sendCommand() {
  Wire.beginTransmission(0x01);
  Wire.write(_command, 14);
  Wire.endTransmission();
}
