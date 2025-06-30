#ifndef SHIFTER_H
#define SHIFTER_H

#include <Arduino.h>
#include <Wire.h>

// I2C communication constants
constexpr uint8_t THRUSTMASTER_I2C_ADDRESS = 0x01;
constexpr size_t COMMAND_BUFFER_SIZE = 14;

// Shifter modes
constexpr uint8_t H_MODE_FLAG = 0x80;

// Commands seen from oscilloscope in ISRTV forum - 
// apparently these codes have changed for different wheel versions however
// these worked for me...
uint8_t command[COMMAND_BUFFER_SIZE] = {
  0x00, // Shifter mode 0 - S / 0x80 - H
  0x0C, // Unknown
  0x01, // Unknown
  0x00, // Gear in H-mode
  0x00, // Gear in S-Mode 0x04 - center, 0x05 - down, 0x06 - up
  0x80, // Unknown
  0x80, // Unknown
  0x00, // Y cordinate
  0x00, // X cordinate
  0x00, // Unknown
  0x00, // Unknown
  0x00, // Unknown
  0x00, // Unknown
  0x00  // Unknown
};

// Sequential shift positions
enum class SequentialPosition : uint8_t {
  CENTER = 0x04,
  DOWN = 0x05,
  UP = 0x06
};

// H mode shifting codes, true = H-mode | false = sequential
void setHMode(const bool isHMode) {
  if (isHMode) {
    command[0] |= H_MODE_FLAG;
  } else {
    command[0] &= ~H_MODE_FLAG;
  }
}

// Switches gear to specified gear number and displays in serial monitor
void switchHGear(const uint8_t gear) { // Gear num 0-N, 8-R
  command[3] = (0x80 >> (8 - gear));
  Serial.print("Gear ");
  Serial.println(gear);
}

// Switches gear to specified gear number and displays in serial monitor
void switchSGear(const SequentialPosition currpos) {
  command[4] = static_cast<uint8_t>(currpos);
  Serial.print("SEQ Gear ");
  Serial.println(static_cast<uint8_t>(currpos));
}

// Sends command over PS2 port to T300 wheelbase
void sendCommand() {
  Wire.beginTransmission(THRUSTMASTER_I2C_ADDRESS);
  Wire.write(command, COMMAND_BUFFER_SIZE);
  Wire.endTransmission();
}

#endif // SHIFTER_H
