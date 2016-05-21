// AudioManager.h

#ifndef _AUDIOMANAGER_h
#define _AUDIOMANAGER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <SD.h>

#define VS1053_DATABUFFERLEN 32

class AudioManagerClass
{
private:

protected:
	boolean _isPlayingAudio;
	boolean _ampEnabled;
	uint8_t _mp3buffer[VS1053_DATABUFFERLEN];
	File    _currentTrack;
	long    _lastAudioFeedTime;

	void feedAudioBuffer();
	boolean readyForAudioData();
	void playAudioData(uint8_t *buffer, uint8_t buffsiz);

	void sciWrite(uint8_t addr, uint16_t data);
	void spiwrite(uint8_t c);
	uint16_t sciRead(uint8_t addr);
	uint8_t spiread(void);
	void dumpRegs(void);

	void softReset();


 public:
	boolean init();
	void reset();

	inline boolean isPlayingAudio() { return _isPlayingAudio; }
	void stopAudio();
	boolean startAudioFile(const char * fileName);
	boolean playFullAudioFile(const char *trackname);
	void setAmpGain(uint8_t level);
	void setVolume(uint8_t left, uint8_t right);

	void updateAudioState();
};

extern AudioManagerClass AudioManager;

#endif

