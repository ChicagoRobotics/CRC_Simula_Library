/*
 Name:		Simula_BehaviorTree.ino
 Created:	6/11/2016 2:05:02 PM
 Author:	jlaing
*/

#include "CRC_Simulation.h"
#include "CRC_AudioManager.h"
#include "CRC_PCA9635.h"
#include "CRC_Lights.h"
#include "CRC_Hardware.h"
#include "CRC_Sensors.h"
#include "BehaviorTree.h"
#include "PingDistance.h"
#include "IR_BinaryDistance.h"
#include "IR_AnalogDistance.h"
#include "DistanceSensor.h"
#include "Motor.h"
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <StandardCplusplus.h>
#include <Adafruit_LSM9DS0.h>	//Download from https://github.com/adafruit/Adafruit_LSM9DS0_Library/archive/master.zip
#include <Adafruit_Sensor.h>	//Download from https://github.com/adafruit/Adafruit_Sensor/archive/master.zip

Sd2Card card;
SdVolume volume;
SdFile root;
File file;

struct HARDWARE_STATE hardwareState;
struct TREE_STATE treeState;

Sensors sensors = Sensors();
CRC_HardwareClass hardware;
CRC_SimulationClass simulation;
Motor motorLeft(hardware.enc1A, hardware.enc1B, hardware.mtr1Enable, hardware.mtr1In1, hardware.mtr1In2);
Motor motorRight(hardware.enc2A, hardware.enc2B, hardware.mtr2Enable, hardware.mtr2In1, hardware.mtr2In2);
Motors motors;
CRC_LightsClass crcLights(hardware.i2cPca9635Left, hardware.i2cPca9635Right);
CRC_AudioManagerClass crcAudio;

Behavior_Tree behaviorTree;
Behavior_Tree::Selector selector[3];
Behavior_Tree::RandomSelector randomSort[1];
Button_Stop buttonStop;
Battery_Check batteryCheck;
Cliff_Center cliffCenter;
Cliff_Left cliffLeft;
Cliff_Right cliffRight;
Perimeter_Center perimeterCenter;
Perimeter_Left perimeterLeft;
Perimeter_Right perimeterRight;
Orientation_Check orientationCheck;
Forward_Random forwardRandom(20);
Turn_Random turnLeft(15, true), turnRight(15, false);
Do_Nothing doNothing(80);

void setup() {
	Serial.begin(115200);
	Serial.println(F("Booting."));

	initializeSystem();
	behaviorTree.setRootChild(&selector[0]);
	selector[0].addChildren({ &buttonStop, &batteryCheck, &orientationCheck, &selector[1], &randomSort[0] });
	selector[1].addChildren({ &perimeterCenter, &perimeterLeft, &perimeterRight, &cliffCenter, &cliffLeft, &cliffRight });
	randomSort[0].addChildren({ &forwardRandom, &doNothing, &turnLeft, &turnRight });

	crcLights.setRandomColor();
	crcLights.showRunwayWithDelay();
	//MP3 Player & Amplifier
	crcAudio.setAmpGain(1); //0 = low, 3 = high
	crcAudio.setVolume(40, 40); //0 = loudest, 60 = softest ?
	
	if (hardware.sdInitialized) {
		//crcAudio.playRandomAudio(F("effects/PwrDn_"), 10, F(".mp3"));
		crcAudio.playRandomAudio(F("effects/PwrUp_"), 10, F(".mp3"));
	}
	Serial.println(F("Setup complete."));
}

void loop() {
	crcAudio.tick();
	simulation.tick();
	if (treeState.treeActive)
	{
		sensors.lsm.read();
		if (!sensors.irReadingUpdated()) {
			sensors.readIR();
		}
	}
	
	if (!behaviorTree.run()) {
		Serial.println(F("All tree nodes returned false."));
	}
}

void initializeSystem() {
	sensors.init();
	Serial.println(F("LSM instantiated."));
	hardware.init();
	Serial.println(F("Hardware initialized."));
	crcLights.init();
	Serial.println(F("Lights initialized."));

	if (crcAudio.init()) {
		hardwareState.audioPlayer = true;
		Serial.println(F("Audio initialized."));
	}
	else {
		Serial.println(F("Audio chip not detected."));
	}
	if (!sensors.lsm.begin())
	{
		Serial.println(F("Oops ... unable to initialize the LSM9DS0. Check your wiring!"));
	}
	else
	{
		Serial.println(F("Setting IMU attributes."));
		// 1.) Set the accelerometer range
		sensors.lsm.setupAccel(sensors.lsm.LSM9DS0_ACCELRANGE_2G);
		// 2.) Set the magnetometer sensitivity
		sensors.lsm.setupMag(sensors.lsm.LSM9DS0_MAGGAIN_2GAUSS);
		// 3.) Setup the gyroscope
		sensors.lsm.setupGyro(sensors.lsm.LSM9DS0_GYROSCALE_245DPS);
		Serial.println(F("IMU configured."));
	}

	//Check battery voltage.
	float postVoltage = hardware.readBatteryVoltage();
	motors.initialize(&motorLeft, &motorRight);

	if (!SD.begin(hardware.sdcard_cs)) {
		Serial.println(F("SD card init failure."));
		hardware.sdInitialized = false;
	}
	else
	{
		Serial.println(F("SD card initialized."));
		hardware.sdInitialized = true;
	}
}


