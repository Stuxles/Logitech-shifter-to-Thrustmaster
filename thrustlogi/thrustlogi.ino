#include <Wire.h>
#include "ThrustmasterBase.h"
#include "LogitechG29Shifter.h"

// Pin definitions
constexpr uint8_t PIN_X_AXIS = A0;
constexpr uint8_t PIN_Y_AXIS = A2;
constexpr uint8_t PIN_REVERSE_BUTTON = 15;
constexpr uint8_t PIN_MODE_SWITCH = 4; // Pin to toggle between H-Pattern and Sequential

const int CONTROLLER_UPDATE_RATE_MS = 50;

ThrustmasterBase wheelBase;
LogitechG29Shifter shifter(PIN_X_AXIS, PIN_Y_AXIS, PIN_REVERSE_BUTTON);

// State variables
int currentGearH = -1;
ThrustmasterBase::SequentialPosition currentPosS = ThrustmasterBase::SequentialPosition::CENTER;

// Mode tracking: 0 = Sequential, 1 = H-Pattern (Default if open with INPUT_PULLUP)
bool currentModeIsH = true;

void setup() {
  Serial.begin(9600);

  // Initialize Shifter
  shifter.begin();

  // Initialize Mode Switch
  pinMode(PIN_MODE_SWITCH, INPUT_PULLUP);

  // Initialize I2C
  Wire.begin(0x03);

  // Initial read of mode
  currentModeIsH = digitalRead(PIN_MODE_SWITCH);

  // Initialize Wheelbase state
  wheelBase.setHMode(currentModeIsH);
  if (currentModeIsH) {
    wheelBase.switchHGear(0);
  } else {
    wheelBase.switchSequentialGear(ThrustmasterBase::SequentialPosition::CENTER);
  }
  wheelBase.sendCommand();
}

void loop() {
  // Read mode switch
  // INPUT_PULLUP: HIGH = Open (Default/H-Pattern?), LOW = Closed (Sequential?)
  // Let's assume Open (HIGH) is H-Pattern (standard behavior) and Closed (LOW) is Sequential.
  bool modeIsH = digitalRead(PIN_MODE_SWITCH);

  // Check for mode change
  if (modeIsH != currentModeIsH) {
    currentModeIsH = modeIsH;
    wheelBase.setHMode(currentModeIsH);

    // Reset states to force update
    currentGearH = -1;
    currentPosS = ThrustmasterBase::SequentialPosition::CENTER;

    Serial.print("Mode changed to: ");
    Serial.println(currentModeIsH ? "H-Pattern" : "Sequential");
  }

  LogitechG29Shifter::Gear g29Gear = shifter.getGear();

  if (currentModeIsH) {
    // H-Pattern Logic
    int gear = static_cast<int>(g29Gear);
    if (currentGearH != gear) {
      currentGearH = gear;
      wheelBase.switchHGear(static_cast<uint8_t>(currentGearH));
      wheelBase.sendCommand();

      Serial.print("H-Pattern Gear: ");
      Serial.println(currentGearH);
    }
  } else {
    // Sequential Logic
    // Logic:
    // Gear 3 (Forward) -> DOWN
    // Gear 4 (Back)    -> UP
    // Else             -> CENTER

    ThrustmasterBase::SequentialPosition targetPos = ThrustmasterBase::SequentialPosition::CENTER;

    if (g29Gear == LogitechG29Shifter::GEAR_3) {
      targetPos = ThrustmasterBase::SequentialPosition::DOWN;
    } else if (g29Gear == LogitechG29Shifter::GEAR_4) {
      targetPos = ThrustmasterBase::SequentialPosition::UP;
    }

    if (currentPosS != targetPos) {
      currentPosS = targetPos;
      wheelBase.switchSequentialGear(currentPosS);
      wheelBase.sendCommand();

      Serial.print("Sequential Pos: ");
      Serial.println(static_cast<int>(currentPosS));
    }
  }

  delay(CONTROLLER_UPDATE_RATE_MS);
}
