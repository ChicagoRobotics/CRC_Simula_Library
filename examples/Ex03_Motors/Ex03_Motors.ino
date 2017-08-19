/***************************************************
This file is designed for the Simula project by Chicago Robotics Corp.
http://www.chicagorobotics.net/products

Copyright (c) 2016, Chicago Robotics Corp.
See README.md for license details
****************************************************/

#include "CRC_Hardware.h"
#include "CRC_Motor.h"

/** Start Instantiate all CRC Modules here **/
CRC_HardwareClass hardware;
CRC_Motor motorLeft(hardware.enc1A, hardware.enc1B, hardware.mtr1Enable, hardware.mtr1In1, hardware.mtr1In2);
CRC_Motor motorRight(hardware.enc2A, hardware.enc2B, hardware.mtr2Enable, hardware.mtr2In1, hardware.mtr2In2);
CRC_Motors motors;
/** End Instantiate all CRC Modules here **/

void setup()
{
  // You can use the Serial Monitor to see these message
  Serial.begin(115200);
  Serial.println(F("Setting up Hardware."));
    
  // Set up the Simula Board
  hardware.init();

  // Configure the motors with left/right motor/encoders
  motors.initialize(&motorLeft, &motorRight);

  Serial.println(F("Motors Ready to go"));
}


void loop()
{
  Serial.println(F("Fowards at full Power"));
  motors.setPower(255, 255); // Forwards
  delay(1000);

  Serial.println(F("Backwards at full Power"));
  motors.setPower(-255, -255); // Backwards
  delay(1000);
  
  Serial.println(F("Spin Right"));
  motors.setPower(255, -255); // Spin right
  delay(1000);

  Serial.println(F("Spin Left"));
  motors.setPower(-255, 255);  // Spin left
  delay(1000);
  
  Serial.println(F("Stop"));
  motors.allStop();  // All Stop, this is the same as  motors.setPower(0, 0);
  delay(1000);
}

