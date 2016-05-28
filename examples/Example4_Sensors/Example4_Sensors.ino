/***************************************************
This file is designed for the Simula project by Chicago Robotics Corp.
http://www.chicagorobotics.net/products

Copyright (c) 2016, Chicago Robotics Corp.
See README.md for license details
****************************************************/


#include "CRC_Hardware.h"
#include "CRC_Globals.h"

/** Start Instantiate all CRC Modules here **/
struct HIVEMSG_STATE UnitState;		// Current state of the Unit
CRC_HardwareClass CRC_Hardware;

/** End Instantiate all CRC Modules here **/


void setup()
{
  CRC_Hardware.init();

}


void loop()
{

}