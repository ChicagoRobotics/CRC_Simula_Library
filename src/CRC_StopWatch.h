/***************************************************
Uses: Provides a Stopwatch/Timer for calculating time differences

This file is designed for the Simula project by Chicago Robotics Corp.
http://www.chicagorobotics.net/products

Inspired By: 
URL: http://playground.arduino.cc/Code/StopWatchClass

Copyright (c) 2016, Chicago Robotics Corp.
See README.md for license details
****************************************************/

#ifndef CRC_StopWatch_h
#define CRC_StopWatch_h

#if ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

class CRC_StopWatch 
{
public:
    enum State { RESET, RUNNING, STOPPED };
    enum Resolution { MILLIS, MICROS, SECONDS };
	CRC_StopWatch(enum Resolution res = MILLIS);
    void start();
    void stop(); 
    void reset();
    unsigned long value();
    unsigned long elapsed() { return value(); };
    bool isRunning();
    enum State state();
    enum Resolution resolution() { return _res; };

	inline void restart() { reset(); start(); }
private:
    enum State _state;
    enum Resolution _res;
    unsigned long _starttime;
    unsigned long _stoptime;
    unsigned long (*_gettime)(void);
    static unsigned long seconds() { return millis()/1000; };
};

#endif
// END OF FILE