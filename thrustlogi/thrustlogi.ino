#include <Wire.h>
#include "shifter.h"

// Shifter analog axis thresholds
// Change these values if the gears aren't detected correctly
#define XAXIS_LEFT_THRESH 350
#define XAXIS_RIGHT_THRESH 680
#define YAXIS_UP_THRESH 700
#define YAXIS_DOWN_THRESH 300

#define GEAR_1 1
#define GEAR_2 2
#define GEAR_3 3
#define GEAR_4 4
#define GEAR_5 5
#define GEAR_6 6
#define GEAR_REVERSE 8
#define GEAR_NEUTRAL 0

//Pin values
#define xPin A0
#define yPin A2
#define buttonPin 15

const int ControllerUpdateRate = 50;
int currentGear;

void setup()
{
    // Shifter analog input pins (make sure they are correctly plugged in Arduino)
    pinMode(xPin, INPUT_PULLUP); // X axis
    pinMode(yPin, INPUT_PULLUP); // Y axis
    pinMode(buttonPin, INPUT);   // Reverse button

    Wire.begin(0x03); // join i2c bus (address optional for master)

    //On startup sends command neutral to initialise this. It isnt nessecary but
    //saves from moving to shift and back. Just be mindful of sequential switch i guess    
    setHMode(true);
    switchHGear(0);
    sendCommand();  
}

void loop()
{
    shift();
    delay(ControllerUpdateRate);
}

void shift()
{
    int x = analogRead(xPin);
    int y = analogRead(yPin);
    int reverse = digitalRead(buttonPin);

    int newGear = GEAR_NEUTRAL;

    if (y >= YAXIS_UP_THRESH)
    {
        if (x <= XAXIS_LEFT_THRESH)
            newGear = GEAR_1;
        if (x > XAXIS_LEFT_THRESH && x < XAXIS_RIGHT_THRESH)
            newGear = GEAR_3;
        if (x >= XAXIS_RIGHT_THRESH)
            newGear = GEAR_5;
    }
    else if (y <= YAXIS_DOWN_THRESH)
    {
        if (x <= XAXIS_LEFT_THRESH)
            newGear = GEAR_2;
        if (x > XAXIS_LEFT_THRESH && x < XAXIS_RIGHT_THRESH)
            newGear = GEAR_4;
        if (x >= XAXIS_RIGHT_THRESH)
        {
            if (!reverse)
                newGear = GEAR_6;
            if (reverse)
                newGear = GEAR_REVERSE;
        }
    }

    if (currentGear != newGear)
    {
        currentGear = newGear;
        
        setHMode(true);
        switchHGear(currentGear);
        sendCommand();
        
        Serial.println(currentGear);
    }
}