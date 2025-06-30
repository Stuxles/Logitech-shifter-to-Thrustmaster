# Logitech shifter to Thrustmaster wheelbase

Arduino project to interface a Logitech shifter with a Thrustmaster wheelbase using modern C++ practices.

## Hardware Requirements

You will need a 3.3v pro micro

| Pro micro | PS/2 plug | DB9 |
|--|--|--|
| VCC |  | 3&7 |
| RAW | 5 |  |
| GND | 3&6 | 6 |
| A0 |  | 4 |
| A2 |  | 8 |
| 2(SDA) | 4 |  |
| 3(SCL) | 2 |  |
| 15 |  | 2 |

The metal shield on the PS/2 plug needs to be connected to ground.

## Code Features

- Modern C++ practices compatible with Arduino framework
- Type-safe enums with `enum class`
- Compile-time constants using `constexpr`
- Proper const correctness
- Comprehensive header guards
- Named constants instead of magic numbers
- Clean code organization and formatting

## Configuration

Analog axis thresholds can be adjusted in `thrustlogi.ino` if gears aren't detected correctly:
- `XAXIS_LEFT_THRESH` / `XAXIS_RIGHT_THRESH`
- `YAXIS_UP_THRESH` / `YAXIS_DOWN_THRESH`

Serial communication baud rate and controller update rate can also be configured.

## Acknowledgments

Made by combining [Logitech -> USB](https://github.com/Vooges/Logitech-USB-Shifter) and [USB -> Thrustmaster](https://github.com/azzajess/USB-Shifter-to-Thrustmaster-Wheelbase)
