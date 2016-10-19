// IR_AnalogDistance.h

#ifndef _IR_ANALOGDISTANCE_h
#define _IR_ANALOGDISTANCE_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#ifndef _DISTANCESENSOR_h
#include "DistanceSensor.h"
#endif // !_DISTANCESENSOR_h

class IR_AnalogDistance : public DistanceSensor {
public:
	IR_AnalogDistance(int activationPin, int readingPin);
	double readDistance();
};

#endif
