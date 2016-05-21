/**
* Helper for working with Flash/Program
* Strings, especially where we have to pass
* Them to functions that take RAM char *
**/

#include "CRC_FlashHelper.h"

char FlashHelper::_mainBuffer[FH_BUFFER_LEN];

char* FlashHelper::main(const __FlashStringHelper * flashString)
{
	strcpy_P(FlashHelper::_mainBuffer, (const char *) flashString);
	return FlashHelper::_mainBuffer;
}

