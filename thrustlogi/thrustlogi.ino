#include <Wire.h>
#include "shifter.h"

// Shifter analog axis thresholds
// Change these values if the gears aren't detected correctly
constexpr int XAXIS_LEFT_THRESH = 365;
constexpr int XAXIS_RIGHT_THRESH = 580;
constexpr int YAXIS_UP_THRESH = 700;
constexpr int YAXIS_DOWN_THRESH = 300;

// Gear definitions
constexpr int GEAR_1 = 1;
constexpr int GEAR_2 = 2;
constexpr int GEAR_3 = 3;
constexpr int GEAR_4 = 4;
constexpr int GEAR_5 = 5;
constexpr int GEAR_6 = 6;
constexpr int GEAR_REVERSE = 8;
constexpr int GEAR_NEUTRAL = 0;

// Pin definitions
constexpr int X_PIN = A0;
constexpr int Y_PIN = A2;
constexpr int BUTTON_PIN = 15;

// I2C and communication constants
constexpr uint8_t I2C_SLAVE_ADDRESS = 0x03;
constexpr int CONTROLLER_UPDATE_RATE = 50;
constexpr long SERIAL_BAUD_RATE = 9600;
int currentGear;

void setup() {
    // Initialize serial communication
    Serial.begin(SERIAL_BAUD_RATE);
    
    // Shifter analog input pins (make sure they are correctly plugged in Arduino)
    pinMode(X_PIN, INPUT_PULLUP); // X axis
    pinMode(Y_PIN, INPUT_PULLUP); // Y axis
    pinMode(BUTTON_PIN, INPUT);   // Reverse button

    Wire.begin(I2C_SLAVE_ADDRESS); // join i2c bus (address optional for master)

    // On startup sends command neutral to initialize this. It isn't necessary but
    // saves from moving to shift and back. Just be mindful of sequential switch    
    setHMode(true);
    switchHGear(GEAR_NEUTRAL);
    sendCommand();  
}

void loop() {
    shift();
    delay(CONTROLLER_UPDATE_RATE);
}

void shift() {
    const int x = analogRead(X_PIN);
    const int y = analogRead(Y_PIN);
    const int reverse = digitalRead(BUTTON_PIN);

    int newGear = GEAR_NEUTRAL;

    if (y >= YAXIS_UP_THRESH) {
        if (x <= XAXIS_LEFT_THRESH) {
            newGear = GEAR_1;
        } else if (x > XAXIS_LEFT_THRESH && x < XAXIS_RIGHT_THRESH) {
            newGear = GEAR_3;
        } else if (x >= XAXIS_RIGHT_THRESH) {
            newGear = GEAR_5;
        }
    } else if (y <= YAXIS_DOWN_THRESH) {
        if (x <= XAXIS_LEFT_THRESH) {
            newGear = GEAR_2;
        } else if (x > XAXIS_LEFT_THRESH && x < XAXIS_RIGHT_THRESH) {
            newGear = GEAR_4;
        } else if (x >= XAXIS_RIGHT_THRESH) {
            if (!reverse) {
                newGear = GEAR_6;
            } else if (reverse) {
                newGear = GEAR_REVERSE;
            }
        }
    }

    if (currentGear != newGear) {
        currentGear = newGear;
        
        setHMode(true);
        switchHGear(currentGear);
        sendCommand();
        
        Serial.println("Current gear: " + String(currentGear));
    }
}