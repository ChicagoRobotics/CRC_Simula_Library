/***************************************************
Uses: Provides higher level functions to control Simula's behavioral simulation.

This file is designed for the Simula project by Chicago Robotics Corp.
http://www.chicagorobotics.net/products

Copyright (c) 2016, Chicago Robotics Corp.
See README.md for license details
****************************************************/

#include "CRC_Simulation.h"

CRC_SimulationClass::CRC_SimulationClass() {
	exertion = 0;
	restingBPM = 60;
	beatMsCheck = 0;
	beatUnderway = false;
	beatFlashDuration = 150;
	beatBrightness = 100;

	restingBreaths = 16; //breaths per minute;
	breathsMsCheck = 0;
	breathBrightness = 0;
	breathFadeTimecheck = millis();

	actionActive = false;
	perimeterActive = false;
	turnSpeed = 160;
	straightSpeed = 180;
}
void CRC_SimulationClass::tick() {
	unsigned long now = millis();
	switch (currentAnimation) {
	case animationBio:
		buttonHeartbeat(now);
		ledBreath(now);
		break;
	case animationRunwayFwd:
		//animationRunwayFwd(now);
		break;
	default:
		break;
	}
}
void CRC_SimulationClass::buttonHeartbeat(unsigned long &now) {
	float beatsPM = ((restingBPM * exertion) / 50) + restingBPM;
	float msPerBeat = 60000 / beatsPM;
	float beatsPerMS = beatsPM / 60000;
	const float beatAmplitude = 64; //This number is max_brightness/2

	if (now - msPerBeat > beatMsCheck)
	{
		beatMsCheck = now;
	}
	unsigned long beatTime = now - beatMsCheck;
	beatBrightness = getSineWave(beatAmplitude, beatsPerMS, beatTime);
	crcLights.setButtonLevel(beatBrightness);
}
void CRC_SimulationClass::ledBreath(unsigned long &now) {
	float breathsPM = ((restingBreaths * exertion) / 50) + restingBreaths;
	float msPerBreath = 60000 / breathsPM;
	float breathsPerMS = breathsPM / 60000;
	const float breathAmplitude = 50;  //this number should be 1/2 the total max value
	
	if (now - msPerBreath > breathsMsCheck)
	{
		breathsMsCheck = now;
		crcLights.setRandomColor();
	}
	unsigned long breathTime = now - breathsMsCheck;
	//breathBrightness = breathAmplitude * sin(breathsPerMS * TWO_PI * breathTime - PI / 2) + breathAmplitude;
	breathBrightness = getSineWave(breathAmplitude, breathsPerMS, breathTime);
	breathFraction = breathBrightness / 100.00;
	crcLights.setAllLeds(crcLights.color_R * breathFraction, crcLights.color_G * breathFraction, crcLights.color_B * breathFraction);
}
void CRC_SimulationClass::showLedNone() {
	crcLights.setAllOff();
	currentAnimation = animationNone;
}
void CRC_SimulationClass::showLedBio() {
	currentAnimation = animationBio;
}
int CRC_SimulationClass::getSineWave(float amplitude, float periodMillis, long millis) {
	//calculation assumes the following:
	//1) Angular frequency = 2PI
	//2) Phase shift = - PI / 2, so we start the wave at zero
	//3) We offset wave by amplitude so output is always zero or above,
	//because LEDs have a hard time displaying negative values.
	//4) periodMillis = wave frequency / millis
	return amplitude * sin(periodMillis * TWO_PI * millis - PI / 2) + amplitude;
}