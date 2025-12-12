# Logitech shifter to Thrustmaster wheelbase

You will need a 3.3v pro micro

| Pro micro | PS/2 plug | DB9 | Description |
|--|--|--|--|
| VCC |  | 3&7 | Power |
| RAW | 5 |  | Power |
| GND | 3&6 | 6 | Ground |
| A0 |  | 4 | X-Axis |
| A2 |  | 8 | Y-Axis |
| 2(SDA) | 4 |  | I2C SDA |
| 3(SCL) | 2 |  | I2C SCL |
| 15 |  | 2 | Reverse Button |
| 4 | | | **Mode Switch** |

The metal shield on the ps2 plug needs to be connected to ground.

## Mode Switching (H-Pattern / Sequential)

This firmware supports both H-Pattern and Sequential shifting modes. You can toggle between them using a physical switch connected to **Pin 4**.

*   **H-Pattern Mode**:
    *   **Switch State**: Open (High / Not connected)
    *   **Behavior**: Standard H-pattern shifting (Gears 1-6 + Reverse).

*   **Sequential Mode**:
    *   **Switch State**: Closed (Low / Connected to Ground)
    *   **Behavior**:
        *   Move stick forward (Gear 3 position) -> **Downshift**
        *   Pull stick back (Gear 4 position) -> **Upshift**

Made by combining [Logitech -> usb](https://github.com/Vooges/Logitech-USB-Shifter) and [usb -> Thrustmaster](https://github.com/azzajess/USB-Shifter-to-Thrustmaster-Wheelbase)
