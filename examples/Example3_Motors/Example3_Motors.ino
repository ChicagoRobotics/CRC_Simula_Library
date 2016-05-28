/***************************************************
This file is designed for the Simula project by Chicago Robotics Corp.
http://www.chicagorobotics.net/products

Copyright (c) 2016, Chicago Robotics Corp.
See README.md for license details
****************************************************/

#include "CRC_Hardware.h"
#include "CRC_Globals.h"
#include "CRC_Motor.h"

/** Start Instantiate all CRC Modules here **/
struct HIVEMSG_STATE UnitState;    // Current state of the Unit
CRC_HardwareClass CRC_Hardware;
CRC_MotorClass CRC_Motor;
/** End Instantiate all CRC Modules here **/

void setup()
{
  CRC_Hardware.init();
}


void loop()
{
  CRC_Motor.setMotor(255, 255); // Forwards
  delay(1000);
  CRC_Motor.setMotor(-255, -255); // Backwards
  delay(1000);
  CRC_Motor.setMotor(255, -255); // Turn right
  delay(1000);
  CRC_Motor.setMotor(-255, 255);  // Turn left
  delay(1000);
  CRC_Motor.setMotor(0, 0);  // All Stop
  delay(1000);
}

