// IR_BinaryDistance.h

#ifndef _IR_BINARYDISTANCE_h
#define _IR_BINARYDISTANCE_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#ifndef _DISTANCESENSOR_h
#include "DistanceSensor.h"
#endif // !_DISTANCESENSOR_h

class IR_BinaryDistance : public DistanceSensor {
public:
	IR_BinaryDistance(int activationPin, int readingPin);
	boolean objectDetected();
};

#endif