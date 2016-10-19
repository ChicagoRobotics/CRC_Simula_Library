// PingDistance.h

#ifndef _PINGDISTANCE_h
#define _PINGDISTANCE_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#ifndef _DISTANCESENSOR_h
#include "DistanceSensor.h"
#endif // !_DISTANCESENSOR_h

class PingDistance : public DistanceSensor {
public:
	PingDistance(int activationPin, int readingPin);
	float readDistance();
};

#endif