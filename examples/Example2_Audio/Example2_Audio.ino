/***************************************************
This file is designed for the Simula project by Chicago Robotics Corp.
http://www.chicagorobotics.net/products

Copyright (c) 2016, Chicago Robotics Corp.
See README.md for license details
****************************************************/

#include "CRC_Hardware.h"
#include "CRC_Globals.h"
#include "CRC_AudioManager.h"

/** Start Instantiate all CRC Modules here **/
struct HIVEMSG_STATE UnitState;		// Current state of the Unit
CRC_HardwareClass CRC_Hardware;
CRC_AudioManagerClass CRC_AudioManager;
/** End Instantiate all CRC Modules here **/

// We have Sounds in the /voices folder of the SD card
uint8_t voice = 0x01;
uint8_t sound = 0x02;

void setup()
{
  CRC_Hardware.init();
  CRC_AudioManager.init();
  CRC_AudioManager.setAmpGain(0);   // Lowest Amp Level (0-3)
  CRC_AudioManager.setVolume(0, 0); // Max Volume  
  CRC_AudioManager.startAudioFile("/voices/01_01.mp3");
}


void loop()
{
  // Feed the Audio buffer, until the file completes
  CRC_AudioManager.updateAudioState();

  if(!CRC_AudioManager.isPlayingAudio())
  {
      delay(3000);
      queueNextFile();
  }
}

void queueNextFile()
{
  // Pick a file int the voices folder
  char fileName[255];

  while(true)
  {
    sprintf(fileName, "/voices/%02d_%02d.mp3", voice, sound ++);
    if(sound > 100)
    {
      voice++;
      sound = 1; 
    }
    if(CRC_AudioManager.startAudioFile(fileName))
    {
      return;
    }
  }
}



