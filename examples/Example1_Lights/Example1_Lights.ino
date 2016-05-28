/***************************************************
This file is designed for the Simula project by Chicago Robotics Corp.
http://www.chicagorobotics.net/products

Copyright (c) 2016, Chicago Robotics Corp.
See README.md for license details
****************************************************/

#include "CRC_Hardware.h"
#include "CRC_Globals.h"
#include "CRC_Lights.h"


/** Start Instantiate all CRC Modules here **/
struct HIVEMSG_STATE UnitState;		// Current state of the Unit
CRC_HardwareClass CRC_Hardware;
CRC_LightsClass CRC_Lights(CRC_Hardware.i2cPca9635Left, CRC_Hardware.i2cPca9635Right);
/** End Instantiate all CRC Modules here **/

void setup()
{
	CRC_Hardware.init();
	CRC_Lights.init();
}


void loop()
{
  int delayTime=300;
  // Just rotate colors all the way through
  for(uint8_t  ledId=0; ledId < 10; ledId++) {
    CRC_Lights.setLed(ledId, 0, 0, 255);
    delay(delayTime);

    CRC_Lights.setLed(ledId, 0, 255, 0);
    delay(delayTime);    

    CRC_Lights.setLed(ledId, 255, 0, 0);
    delay(delayTime);        
    
    CRC_Lights.setLed(ledId, 0, 255, 255);
    delay(delayTime);

    CRC_Lights.setLed(ledId, 255, 255, 0);
    delay(delayTime);    

    CRC_Lights.setLed(ledId, 255, 255, 255);
    delay(delayTime);    

    CRC_Lights.setLed(ledId, 0, 0, 0);
  }
}
