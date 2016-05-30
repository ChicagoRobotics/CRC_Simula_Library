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

CRC_PCA9635 leftLedBank(CRC_Hardware.i2cPca9635Left);
CRC_PCA9635 rightLedBank(CRC_Hardware.i2cPca9635Right);
CRC_Lights lights(leftLedBank, rightLedBank);
/** End Instantiate all CRC Modules here **/

void setup()
{
	CRC_Hardware.init();
	lights.init();
}


void loop()
{
  int delayTime=300;
  // Just rotate colors all the way through
  for(uint8_t  ledId=0; ledId < 10; ledId++) {
    lights.setLed(ledId, 0, 0, 255);
    delay(delayTime);

    lights.setLed(ledId, 0, 255, 0);
    delay(delayTime);    

    lights.setLed(ledId, 255, 0, 0);
    delay(delayTime);        
    
    lights.setLed(ledId, 0, 255, 255);
    delay(delayTime);

    lights.setLed(ledId, 255, 255, 0);
    delay(delayTime);    

    lights.setLed(ledId, 255, 255, 255);
    delay(delayTime);    

    lights.setLed(ledId, 0, 0, 0);
  }
}
