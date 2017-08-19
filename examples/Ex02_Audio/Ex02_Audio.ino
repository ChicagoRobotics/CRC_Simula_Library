/***************************************************
This file is designed for the Simula project by Chicago Robotics Corp.
http://www.chicagorobotics.net/products

Copyright (c) 2016, Chicago Robotics Corp.
See README.md for license details
****************************************************/

#include <SD.h>
#include "CRC_Hardware.h"
#include "CRC_AudioManager.h"

/** Start Instantiate all CRC Modules here **/
HARDWARE_STATE hardwareState;
CRC_HardwareClass hardware;
CRC_AudioManagerClass crcAudio;
/** End Instantiate all CRC Modules here **/


void setup()
{
  Serial.begin(115200);
  Serial.println(F("Booting."));

  hardware.init();
  Serial.println(F("Hardware initialized."));

  // The Audio Manager Works with the SD Card. Make sure it is initialized
  if (!SD.begin(hardware.sdcard_cs)) {
    Serial.println(F("SD card init failure."));
    hardware.sdInitialized = false;
  }
  else
  {
    Serial.println(F("SD card initialized."));
    hardware.sdInitialized = true;
  }
    
  // Make sure the Audio Manager is initialized
  if (crcAudio.init()) {
    hardwareState.audioPlayer = true;
    Serial.println(F("Audio initialized."));
  }
  else {
    Serial.println(F("Audio chip not detected."));
  }  
    
  //Amplifier gain and VS1053 volume
  crcAudio.setAmpGain(2); //0 = low, 3 = high
  crcAudio.setVolume(20, 20); //0 = loudest, 60 = softest
  Serial.println(F("Setup complete."));


  // Demo some methods here  

  // Demo playing a file to completion
  Serial.println(F("Playing blocking audio, next log message will display only after audio completes"));      
  crcAudio.playFullAudioFile("voices/01_01.mp3");
  
  // Demo what happens when a file is not found, it will just ignore
  Serial.println(F("Trying to play a non-existent file"));
  crcAudio.startAudioFile("notafile/doesnotexist.mp3");

  // Demo playing a file, this is non-blocking, 
  Serial.println(F("Play an existing file, this is non-blocking, and will just queue the audio file to be played in the loop"));  
  crcAudio.startAudioFile("voices/01_02.mp3");
  Serial.println(F("Started Audio File, entering Loop"));  
}

void loop()
{
  // Feed the Audio buffer, until the file completes
  // Typically we do this at the start of the Loop, regardless of whether audio is playing or not
  crcAudio.tick();

  // If the Audio is no longer playing, pick a new file to play
  if(!crcAudio.isPlayingAudio())
  {
      delay(3000);
      queueNextFile();
  }
}


// Create a mapping of the Default SD Card's Files so we can randomly cycle through them
struct VOICES_DEFINITION {
  char* filePrefix; 
  int    numFiles;
};
static const VOICES_DEFINITION AUDIO_FILE_MAPPINGS[] = {
  // Effects Samples
  {"effects/Alarm_", 10},
  {"effects/Calc_", 11},
  {"effects/Drone_", 7},
  {"effects/Error_", 11},
  {"effects/Gltch_", 12},
  {"effects/Hlgrm_", 8},    
  {"effects/Notif_", 13},
  {"effects/PwrDn_", 10},
  {"effects/PwrUp_", 10},        
  {"effects/Scrll_", 11},          
  {"effects/Selct_", 13},            
  {"effects/SwcPt_", 8},    
  {"effects/Swipe_", 11},     
  {"effects/Text_", 7},       
  {"effects/ZmIn_", 11},     
  {"effects/ZmOut_", 11},

  // Emotions Samples
  {"emotions/anger_", 6},  
  {"emotions/beeps_", 6},  
  {"emotions/happy_", 9},  
  {"emotions/pain_", 8},  
  {"emotions/sad_", 5},  
  {"emotions/scare_", 9},  

  // If a file is not found, its not a problem
  {"notfound/notafile_", 10}            
            
};

int currentMapping = 0;
const int totalMappings = sizeof(AUDIO_FILE_MAPPINGS)/ sizeof(AUDIO_FILE_MAPPINGS[0]);

/**
 * The default SD Card comes with a voice palette you can select from
 */
void queueNextFile()
{
  Serial.print(F("Playing random File From: "));
  Serial.println(AUDIO_FILE_MAPPINGS[currentMapping].filePrefix);
   
  // Select one of the samples from the SD Card randomly
  crcAudio.playRandomAudio(AUDIO_FILE_MAPPINGS[currentMapping].filePrefix, AUDIO_FILE_MAPPINGS[currentMapping].numFiles, F(".mp3"));

  currentMapping ++;
  if(currentMapping == totalMappings) {
    // Start over
    currentMapping = 0;
  }  
}






