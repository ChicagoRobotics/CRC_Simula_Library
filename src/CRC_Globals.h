/**
Globals.h

Declaration of all global variables
**/

#include <stdint.h>
#ifndef _GLOBALS_h
#define _GLOBALS_h

#include "CRC_MessageTypes.h"

// Unit Id. This only applies for Wireless Zigbee (802.15.4)
// For Wifi, we have an alternate identification method. (MAC etc.)
// Later to become 
extern uint8_t g_intUnitID;

// The Current State of the Unit
extern struct HIVEMSG_STATE UnitState;

// Data Transfer State Definitions
static const uint8_t COM_START = 0;  // Waiting for Message
static const uint8_t COM_ADDRESS = 1; // Waiting for Message Address
static const uint8_t COM_TRANSFER = 2; // Address Verified, Reading message
static const uint8_t COM_COMPLETED = 3; // Message Completed and verified

static const char*  BOOL_TRUE = "true";
static const char*  BOOL_FALSE= "false";

// Wireless State Definitions
static const uint8_t WI_STATUS_NOTAVAILABLE = 0x00;
static const uint8_t WI_STATUS_WIFI_IDLE = 0x01;
static const uint8_t WI_STATUS_WIFI_CONNECTED = 0x02;
static const uint8_t WI_STATUS_WIFI_NOTAVAILABLE = 0x03;

#endif

