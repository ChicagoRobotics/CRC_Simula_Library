/**
MessageTypes.h

Declaration of all the Communication Message Types
**/

#ifndef _MESSAGETYPES_h
#define _MESSAGETYPES_h

#include <stdint.h>

// All structs aligned on byte boundary
#pragma pack(push, 1)

// Maximum file path, inclusive of Folder and null terminator
#define HIVEUNIT_MAX_FILE_PATH 61

// Messages to Hive Unit
#define HIVEMSG_MOVE_ID		0x00
struct HIVEMSG_MOVE
{
	int8_t leftMotor;    // -100 -> 100
	int8_t rightMotor;   // -100 -> 100
};

// Query the Hive Unit for info
#define HIVEMSG_QUERY_ID	0x01
struct HIVEMSG_QUERY
{
	uint8_t queryType;    // 0x00 = STATUS (Returns 0x03)
};

// Messages from Hive Unit
#define HIVEMSG_ACK_ID		0x02
#define HIVEMSG_ACK_OK			0
#define HIVEMSG_ACK_UNKNOWN		1
#define HIVEMSG_ACK_FAILED      2
#define HIVEMSG_ACK_READY       3

#define ERR_FILE_NOT_FOUND       0x01
#define ERR_BUSY                 0x02
#define ERR_NOTAVAILABLE         0x03
#define ERR_INVALID_PARAM        0x03

struct HIVEMSG_ACK
{
	int8_t sequence;    // Sequence of recieved message being acknowledged
	uint8_t messageId;  // Id of message being acknowledged
	uint8_t ackCode;    // The status of the ack
	uint8_t ackParam;     // Arbitrary parameter for the ack
};

#define HIVEMSG_TEXT_ID	0x03
struct HIVEMSG_TEXT
{
	char text[61];
};

#define HIVEMSG_STATE_ID    0x04
struct HIVEMSG_STATE
{
	uint16_t totalRam = 0;      // Total RAM in bytes
	uint16_t freeRam = 0;       // Free RAM in bytes
	int8_t leftMotor  = 0;       // -100 -> 100
	int8_t rightMotor = 0;       // -100 -> 100	
	boolean sdCard = false;      // SD Card initialized/available state
	boolean audioPlayer = false; // Audi Player state
	boolean audioPlaying = false; // Is the audio player playing
	uint8_t wireless = 0x00;     // Wireless Status
	unsigned long loopLastTimeMillis = 0; // Last Time in millis
	unsigned long loopMinTimeMillis  = 0;  // Min Time in millis
	unsigned long loopMaxTimeMillis  = 0;  // Max Time in millis

	// Sensors
	boolean irLeft;
	boolean irFrontLeft;
	boolean irFrontRight;
	boolean irRight;
	boolean irFrontLeftEdge;
	boolean irFrontRightEdge;

	uint8_t irFrontCM;     // Front IR CM Reading
	uint8_t pingFrontCM;   // Front Ping CM Reading
	uint8_t frontCM;       // Effective From CM Reading
	uint16_t irFrontRaw;   // Front IR Raw Reading
	uint16_t pingFrontRaw; // Front Ping Raw Reading
};


#define HIVEMSG_MUSIC_PLAY_ID     0x05
#define HIVEMSG_MUSIC_CMD_PLAY    0x00
#define HIVEMSG_MUSIC_CMD_REQUEST 0x01
struct HIVEMSG_MUSIC_PLAY
{
	/**
	* 0x01 Play			Start playing. Replace current song if currently playing.
	* 0x02 Request		Start playing, only if nothing else is playing
	**/
	uint8_t playType = 0x01;      
	char filePath[HIVEUNIT_MAX_FILE_PATH];
};

#define HIVEMSG_MUSIC_CONTROL_ID    0x06
#define HIVEMSG_MUSIC_CMD_STOP      0x00
#define HIVEMSG_MUSIC_CMD_VOLUME    0x01
#define HIVEMSG_MUSIC_CMD_AMP       0x02
struct HIVEMSG_MUSIC_CONTROL
{
	/**
	* 0x00 Stop			Stop playing
	* 0x01 Volume       Set Volume
	* 0x02 Amp          Set Amplifier Level
	**/
	uint8_t cmd = 0x00;
	uint8_t value1;      // Value to Set for Volume/Amp Control
	uint8_t value2;      // Value to Set for Volume/Amp Control
};


#define HIVEMSG_LED_CONTROL_ID      0x07
#define HIVEMSG_LED_CMD_SET         0x00
struct HIVEMSG_LED_CONTROL
{
	/**
	* 0x00 Set          Set LED to specified levels/colors
	**/
	uint8_t cmd = 0x00;
	uint8_t ledId;		// Id of the Led
	uint8_t red;		
	uint8_t green;      
	uint8_t blue;
};

#define HIVEMSG_FILE_CONTROL_ID      0x08
#define HIVEMSG_FILE_CMD_LS          0x00
#define HIVEMSG_FILE_CMD_RM          0x01
#define HIVEMSG_FILE_CMD_MKDIR       0x02
#define HIVEMSG_FILE_CMD_CAT         0x03
#define HIVEMSG_FILE_CMD_GET         0x04
#define HIVEMSG_FILE_CMD_PUT         0x05
#define HIVEMSG_FILE_CMD_INFO        0x06

#define HIVEMSG_FILETYPE_FILE        0x00
#define HIVEMSG_FILETYPE_DIR         0x01
struct HIVEMSG_FILE_CONTROL
{
	uint8_t cmd = 0x00;
	uint32_t fileSize;     // Size of the File in Bytes
	uint8_t  fileType;     // Type of the File (0=File, 1=Directory)
	char filePath[HIVEUNIT_MAX_FILE_PATH];
};

// Streaming stuff to someone else on request
#define HIVEMSG_FILE_STREAM_ID       0x09
struct HIVEMSG_FILE_STREAM
{
	uint8_t messageSequence;	// Stream Request Message Sequence
	uint8_t messageId;          // Stream Request Message Id
	uint32_t fileSize;          // Size of the File in Bytes
};


#pragma pack(pop)
#endif // _MESSAGETYPES_h
