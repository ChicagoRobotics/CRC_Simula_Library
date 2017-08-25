/***************************************************
  This file is designed for the Simula project by Chicago Robotics Corp.
  http://www.chicagorobotics.net/products

  Copyright (c) 2016, Chicago Robotics Corp.
  See README.md for license details
****************************************************/

#include "CRC_Hardware.h"
#include "CRC_Sensors.h"
#include "CRC_Lights.h"

/** Start Instantiate all CRC Modules here **/
CRC_HardwareClass hardware;
CRC_Sensors sensors = CRC_Sensors();

// We will use the lights to highlight the sensor readings
CRC_LightsClass crcLights(hardware.i2cPca9635Left, hardware.i2cPca9635Right);
/** End Instantiate all CRC Modules here **/

const uint8_t LED_L1 = 0; // Note, these are in the back of Simula
const uint8_t LED_L2 = 1; // and are numbered from back to front
const uint8_t LED_L3 = 2;
const uint8_t LED_L4 = 3;
const uint8_t LED_L5 = 4;

const uint8_t LED_R1 = 5; // Note, these are in the back of Simula
const uint8_t LED_R2 = 6; // and are numbered from back to front
const uint8_t LED_R3 = 7;
const uint8_t LED_R4 = 8;
const uint8_t LED_R5 = 9;

void setup()
{
  // You can use the Serial Monitor to see these message
  Serial.begin(115200);
  Serial.println(F("Setting up Hardware."));

  // Set up the Simula Board
  hardware.init();

  // Activate the Sensors
  sensors.activate();

  // Init the lights, so we can also use them to show the sensor readings
  crcLights.init();

  Serial.println(F("Ready"));
}


void loop()
{
  // Read the sensors in a Loop.
  sensors.readIR();

  // Check the Cliff Detection Flags
  // You can lift the Front of the Simula Unit off the table and view the output in the Serial Monitor
  // The Back LEDs will show red on each side that it detects a cliff.
  checkCliffDetection("Left Cliff detected", sensors.irLeftCliff, LED_L1);
  checkCliffDetection("Right Cliff detected", sensors.irRightCliff, LED_R1); // Note, these are in the back of Simula

  // Check the IR and Ping Sensor Readings.
  // You can add obstacled at varying distances to these sensors and see the readings in the Serial Monitor
  // This will also light up different LEDs to intensities based on the Proximity (Closer = Brighter)
  distanceReading("IR Left", sensors.irLeftCM, LED_L3);
  distanceReading("IR Right", sensors.irRightCM, LED_R3);

  distanceReading("IR Front Left", sensors.irLeftFrontCM, LED_L4);
  distanceReading("IR Front Right", sensors.irRightFrontCM, LED_R4);

  distanceReading("IR Front", sensors.irFrontCM, LED_L5);
  distanceReading("Ping Front", sensors.pingFrontCM, LED_R5);

  delay(500);
}

/**
   Checks to see if a Cliff is detected
*/
void checkCliffDetection(char * message, boolean cliffDetectionValue, uint8_t ledToLightUp)
{
  if (cliffDetectionValue) {
    Serial.println(message);
    crcLights.setLed(ledToLightUp, 255, 0, 0); // Red alert!
  } else {
    crcLights.setLed(ledToLightUp, 0, 0, 0); // All is good
  }
}


/**
   Checks the distance value and displays it accordingly
*/
void distanceReading(char * message, uint8_t distanceCM, uint8_t ledToLightUp)
{
  Serial.print(message);
  Serial.print(":");
  Serial.println(distanceCM);

  // Light up the LED with increased intensity based on how close something is
  // And leave it off after a certain distance
  uint8_t maxThreshold = 50; // 50 cm distance

  if (distanceCM == 0 || distanceCM >= maxThreshold)
  {
    crcLights.setLed(ledToLightUp, 0, 0, 0); // Turn the lights off.
  } else
  {
    uint8_t intensity = (maxThreshold - distanceCM) * 255 / maxThreshold;
    crcLights.setLed(ledToLightUp, 0, intensity, 0); // Turn up the green
  }


}

