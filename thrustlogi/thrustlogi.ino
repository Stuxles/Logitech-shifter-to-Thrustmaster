#include <Wire.h>
#include "ThrustmasterBase.h"
#include "LogitechG29Shifter.h"

// Pin definitions
constexpr uint8_t PIN_X_AXIS = A0;
constexpr uint8_t PIN_Y_AXIS = A2;
constexpr uint8_t PIN_REVERSE_BUTTON = 15;

const int CONTROLLER_UPDATE_RATE_MS = 50;

ThrustmasterBase wheelBase;
LogitechG29Shifter shifter(PIN_X_AXIS, PIN_Y_AXIS, PIN_REVERSE_BUTTON);

int currentGear = -1; // Initialize to invalid gear to force first update

void setup() {
  Serial.begin(9600); // Initialize Serial for debugging

  // Initialize Shifter
  shifter.begin();

  // Initialize I2C
  // Join i2c bus (address optional for master).
  // Original code used 0x03 as slave address but acted as master.
  Wire.begin(0x03);

  // Initialize Wheelbase state
  // On startup sends command neutral to initialise this.
  wheelBase.setHMode(true);
  wheelBase.switchHGear(0);
  wheelBase.sendCommand();
}

void loop() {
  LogitechG29Shifter::Gear newGearEnum = shifter.getGear();
  int newGear = static_cast<int>(newGearEnum);

  if (currentGear != newGear) {
    currentGear = newGear;

    wheelBase.setHMode(true);
    wheelBase.switchHGear(static_cast<uint8_t>(currentGear));
    wheelBase.sendCommand();

    Serial.print("Gear changed to: ");
    Serial.println(currentGear);
  }

  delay(CONTROLLER_UPDATE_RATE_MS);
}
