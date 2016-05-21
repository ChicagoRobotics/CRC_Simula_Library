/**
* Helper for working with Flash/Program
* Strings, especially where we have to pass
* Them to functions that take RAM char *
**/

#ifndef _FLASHHELPER_h
#define _FLASHHELPER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define FHM(string_literal) (FlashHelper::main(F(string_literal)))

#define FH_BUFFER_LEN 255
class FlashHelper
{
private:
	// These are Temp buffers in RAM 
	// Used to transfer the text from Flash

	static char _mainBuffer[FH_BUFFER_LEN]; // Buffer invoked from Main code

public:
	// Do not invoke from an IRQ or chain in same call
	static char* main(const __FlashStringHelper * flashString);
};


#endif

