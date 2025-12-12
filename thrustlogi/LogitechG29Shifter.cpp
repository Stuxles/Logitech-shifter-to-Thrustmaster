#include "LogitechG29Shifter.h"

LogitechG29Shifter::LogitechG29Shifter(uint8_t xPin, uint8_t yPin, uint8_t buttonPin)
    : _xPin(xPin), _yPin(yPin), _buttonPin(buttonPin) {}

void LogitechG29Shifter::begin() {
  pinMode(_xPin, INPUT_PULLUP);
  pinMode(_yPin, INPUT_PULLUP);
  // Original code used INPUT for buttonPin, implying external pull-down?
  // But common mods use INPUT_PULLUP (switch to ground).
  // I will stick to INPUT to match original behavior, assuming external circuitry or internal behavior logic.
  // Wait, if it's INPUT, it is floating unless externally pulled.
  // The logic check `!reverse` (where reverse = digitalRead) implies:
  // If `reverse` is LOW (0) -> `!reverse` is TRUE -> Gear 6.
  // If `reverse` is HIGH (1) -> `!reverse` is FALSE -> Reverse.
  //
  // Usually, pushing the stick down (engaging switch) means Reverse.
  // If the switch connects to Ground (common):
  // Pressed = Low. Released = High (if pulled up).
  // If Released (High), `!reverse` is False -> Reverse.
  // If Pressed (Low), `!reverse` is True -> Gear 6.
  // This means Released = Reverse. This is backwards.
  //
  // If the switch connects to VCC (active high) and pulled down:
  // Pressed = High. Released = Low.
  // If Released (Low), `!reverse` is True -> Gear 6.
  // If Pressed (High), `!reverse` is False -> Reverse.
  // This matches expected behavior: Released = 6, Pressed = Reverse.
  //
  // So the original code assumes Active High button (Pulled Down).
  pinMode(_buttonPin, INPUT);
}

LogitechG29Shifter::Gear LogitechG29Shifter::getGear() {
  int x = analogRead(_xPin);
  int y = analogRead(_yPin);
  int reverseBtn = digitalRead(_buttonPin);

  if (y >= YAXIS_UP_THRESH) {
    if (x <= XAXIS_LEFT_THRESH) {
      return GEAR_1;
    } else if (x > XAXIS_LEFT_THRESH && x < XAXIS_RIGHT_THRESH) {
      return GEAR_3;
    } else if (x >= XAXIS_RIGHT_THRESH) {
      return GEAR_5;
    }
  } else if (y <= YAXIS_DOWN_THRESH) {
    if (x <= XAXIS_LEFT_THRESH) {
      return GEAR_2;
    } else if (x > XAXIS_LEFT_THRESH && x < XAXIS_RIGHT_THRESH) {
      return GEAR_4;
    } else if (x >= XAXIS_RIGHT_THRESH) {
      // Logic fix:
      // If button is NOT pressed (assuming Active High: Low): `!reverseBtn` is True -> Gear 6.
      // If button IS pressed (assuming Active High: High): `!reverseBtn` is False -> Reverse.
      if (!reverseBtn) {
        return GEAR_6;
      } else {
        return REVERSE;
      }
    }
  }

  return NEUTRAL;
}
