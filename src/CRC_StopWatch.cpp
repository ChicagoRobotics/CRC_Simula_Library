/***************************************************
Uses: Provides a Stopwatch/Timer for calculating time differences

This file is designed for the Simula project by Chicago Robotics Corp.
http://www.chicagorobotics.net/products

Inspired By:
URL: http://playground.arduino.cc/Code/StopWatchClass

Copyright (c) 2016, Chicago Robotics Corp.
See README.md for license details
****************************************************/

#include "CRC_StopWatch.h"

CRC_StopWatch::CRC_StopWatch(enum Resolution res)
{
    _res = res;
    switch(_res) {
        case MICROS:
            _gettime = micros;
            break;
        case MILLIS:
            _gettime = millis;
            break;
        case SECONDS:
            _gettime = seconds;
            break;
        default:  
            _gettime = millis;
            break;
    }
    reset();
}

void CRC_StopWatch::reset()
{
    _state = CRC_StopWatch::RESET;
    _starttime = _stoptime = 0;
}

void CRC_StopWatch::start()
{
    if (_state == CRC_StopWatch::RESET || _state == CRC_StopWatch::STOPPED)
    {
        _state = CRC_StopWatch::RUNNING;
        unsigned long t = _gettime();
        _starttime += t - _stoptime;
        _stoptime = t;
    }
}

unsigned long CRC_StopWatch::value()
{
    if (_state == CRC_StopWatch::RUNNING) _stoptime = _gettime();
    return _stoptime - _starttime;
}

void CRC_StopWatch::stop()
{
    if (_state == CRC_StopWatch::RUNNING)
    {
        _state = CRC_StopWatch::STOPPED;
        _stoptime = _gettime();
    }
}

bool CRC_StopWatch::isRunning()
{
    return (_state == CRC_StopWatch::RUNNING);
}

enum CRC_StopWatch::State CRC_StopWatch::state()
{
    return _state;
}
// END OF FILE