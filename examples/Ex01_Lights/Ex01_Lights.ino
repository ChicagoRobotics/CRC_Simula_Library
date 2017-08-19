/***************************************************
This file is designed for the Simula project by Chicago Robotics Corp.
http://www.chicagorobotics.net/products

Copyright (c) 2016, Chicago Robotics Corp.
See README.md for license details


Example Notes:
* There are 10 LEDs mapped into the unit. 5 Left and 5 Right LEDs.
* Each of the 10 LEDs is a RGB LED, and is accessed by Index into the CRC_Lights module
* L1: 0, L2: 1, L3: 2, L4: 3, L5:4
* R1: 5, R2: 6, R3: 7, R4: 8, R5: 9
****************************************************/

#include "CRC_Hardware.h"
#include "CRC_Lights.h"


/** Start Instantiate all CRC Modules here **/
CRC_HardwareClass hardware;
CRC_LightsClass crcLights(hardware.i2cPca9635Left, hardware.i2cPca9635Right);
/** End Instantiate all CRC Modules here **/

/**
 * Initialize all Modules in the setup function
 */
void setup()
{
  // You can use the Serial Monitor to see these message
  Serial.begin(115200);
  Serial.println(F("Booting."));

  hardware.init();
  Serial.println(F("Hardware initialized."));
  
  crcLights.init();
  Serial.println(F("Lights module initialized"));  

  Serial.println(F("Setup complete."));
}

/**
 * Demo's various LED effects in a loop. You can comment each of these functions out to explore the specific sample behaviors
 */
void loop()
{  
  runwayEffects();  
  fadeInOutButton();  
  cycleLeds();
  fadeInOutLeds();
}

/**
 * This is a special effects method with pre-defined behavior. It is typically used on startup since its a blocking method.
 */
void runwayEffects()
{
  Serial.println(F("Start - Demonstrating Runway Effects"));
  crcLights.setRandomColor(); // Init runway
  crcLights.showRunwayWithDelay(); // Demo random runway lights

    Serial.println(F("Completed - Demonstrating Runway Effects"));
}

/**
 * Fade the Top Button In/Out
 */
void fadeInOutButton() 
{
  Serial.println(F("Start - Fading In/Out Top Button"));
    
  uint8_t intensity = 0;
  int dir = 1; 
  while(! (intensity == 0 && dir == -1) ) {
    crcLights.setButtonLevel(intensity);
    delay(10);
    if(intensity == 255 && dir== 1)
      dir = -1;
    else if (intensity == 0 && dir == -1)
      break;
      
    intensity += dir;
  }

    Serial.println(F("Completed - Fading In/Out Top Button"));
}

/**
 * Cycle each LED one at a time
 */
void cycleLeds() 
{
  Serial.println(F("Start - Cycling all LEDs"));
    
  int delayTime=300;
  // Just rotate colors all the way through
  for(uint8_t  ledId=0; ledId < 10; ledId++) {
    crcLights.setLed(ledId, 0, 0, 255);
    delay(delayTime);

    crcLights.setLed(ledId, 0, 255, 0);
    delay(delayTime);    

    crcLights.setLed(ledId, 255, 0, 0);
    delay(delayTime);        
    
    crcLights.setLed(ledId, 0, 255, 255);
    delay(delayTime);

    crcLights.setLed(ledId, 255, 255, 0);
    delay(delayTime);    

    crcLights.setLed(ledId, 255, 255, 255);
    delay(delayTime);    

    crcLights.setLed(ledId, 0, 0, 0);
  }

  Serial.println(F("Completed - Cycling all LEDs"));  
}


/**
 * Fade all Leds in/out 1 color at a time.
 */
void fadeInOutLeds() 
{
  Serial.println(F("Start - Fade In/Out LEDs"));
    
  uint8_t intensity;
  int dir;

  // Fade Up/Down Red
  intensity = 0;
  dir = 1;   
  Serial.println(F("    - Fade In/Out Red"));
  while(! (intensity == 0 && dir == -1) ) {
    for(uint8_t  ledId=0; ledId < 10; ledId++) {
      crcLights.setLed(ledId, intensity, 0, 0);
    }
    if(intensity == 255 && dir== 1)
      dir = -1;
      
    intensity += dir;
  }



  // Fade Up/Down Green
  intensity = 0;
  dir = 1;  
  Serial.println(F("    - Fade In/Out Green"));
  while(! (intensity == 0 && dir == -1) ) {
    for(uint8_t  ledId=0; ledId < 10; ledId++) {
      crcLights.setLed(ledId, 0, intensity, 0);
    }
    if(intensity == 255 && dir== 1)
      dir = -1;
    else if (intensity == 0 && dir == -1)
      break;
      
    intensity += dir;
  }


  // Fade Up/Down Blue
  intensity = 0;
  dir = 1;     
  Serial.println(F("    - Fade In/Out Blue"));
  while(! (intensity == 0 && dir == -1) ) {
    for(uint8_t  ledId=0; ledId < 10; ledId++) {
      crcLights.setLed(ledId, 0, 0, intensity);
    }
    if(intensity == 255 && dir== 1)
      dir = -1;
    else if (intensity == 0 && dir == -1)
      break;
      
    intensity += dir;
  }


  // Fade Up/Down Red+Green
  intensity = 0;
  dir = 1;     
  Serial.println(F("    - Fade In/Out Red+Green"));
  while(! (intensity == 0 && dir == -1) ) {
    for(uint8_t  ledId=0; ledId < 10; ledId++) {
      crcLights.setLed(ledId, intensity, intensity, 0);
    }
    if(intensity == 255 && dir== 1)
      dir = -1;
    else if (intensity == 0 && dir == -1)
      break;
      
    intensity += dir;
  }



  // Fade Up/Down Red+Blue
  intensity = 0;
  dir = 1;     
  Serial.println(F("    - Fade In/Out Red+Blue"));
  while(! (intensity == 0 && dir == -1) ) {
    for(uint8_t  ledId=0; ledId < 10; ledId++) {
      crcLights.setLed(ledId, intensity, 0, intensity);
    }
    if(intensity == 255 && dir== 1)
      dir = -1;
    else if (intensity == 0 && dir == -1)
      break;
      
    intensity += dir;
  }  


  // Fade Up/Down Green+Blue
  intensity = 0;
  dir = 1;     
  Serial.println(F("    - Fade In/Out Green+Blue"));
  while(! (intensity == 0 && dir == -1) ) {
    for(uint8_t  ledId=0; ledId < 10; ledId++) {
      crcLights.setLed(ledId, 0, intensity, intensity);
    }
    if(intensity == 255 && dir== 1)
      dir = -1;
    else if (intensity == 0 && dir == -1)
      break;
      
    intensity += dir;
  }  


  // Fade Up/Down Red+Green+Blue
  intensity = 0;
  dir = 1;     
  Serial.println(F("    - Fade In/Out Red+Green+Blue"));
  while(! (intensity == 0 && dir == -1) ) {
    for(uint8_t  ledId=0; ledId < 10; ledId++) {
      crcLights.setLed(ledId, intensity, intensity, intensity);
    }
    if(intensity == 255 && dir== 1)
      dir = -1;
    else if (intensity == 0 && dir == -1)
      break;
      
    intensity += dir;
  }

  Serial.println(F("Completed - Fade In/Out LEDs"));  
}    

