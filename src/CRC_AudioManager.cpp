// 
// This is implemented, talking directly to the VS1053 Chip. Eventually,
// we will want to make the VS1053 code as a separate module. 
// 

#include "CRC_AudioManager.h"
#include "CRC_Hardware.h"
#include <SPI.h>

AudioManagerClass AudioManager;

// START VS1053 Definitions (TODO, delete/comment out what is not used)
#define VS1053_REG_MODE  0x00
#define VS1053_SCI_READ 0x03
#define VS1053_SCI_WRITE 0x02

#define VS1053_REG_MODE  0x00
#define VS1053_REG_STATUS 0x01
#define VS1053_REG_BASS 0x02
#define VS1053_REG_CLOCKF 0x03
#define VS1053_REG_DECODETIME 0x04
#define VS1053_REG_AUDATA 0x05
#define VS1053_REG_WRAM 0x06
#define VS1053_REG_WRAMADDR 0x07
#define VS1053_REG_HDAT0 0x08
#define VS1053_REG_HDAT1 0x09
#define VS1053_REG_VOLUME 0x0B

#define VS1053_GPIO_DDR 0xC017
#define VS1053_GPIO_IDATA 0xC018
#define VS1053_GPIO_ODATA 0xC019

#define VS1053_INT_ENABLE  0xC01A

#define VS1053_MODE_SM_DIFF 0x0001
#define VS1053_MODE_SM_LAYER12 0x0002
#define VS1053_MODE_SM_RESET 0x0004
#define VS1053_MODE_SM_CANCEL 0x0008
#define VS1053_MODE_SM_EARSPKLO 0x0010
#define VS1053_MODE_SM_TESTS 0x0020
#define VS1053_MODE_SM_STREAM 0x0040
#define VS1053_MODE_SM_SDINEW 0x0800
#define VS1053_MODE_SM_ADPCM 0x1000
#define VS1053_MODE_SM_LINE1 0x4000
#define VS1053_MODE_SM_CLKRANGE 0x8000
#define VS1053_SCI_AIADDR 0x0A
#define VS1053_SCI_AICTRL0 0x0C
#define VS1053_SCI_AICTRL1 0x0D
#define VS1053_SCI_AICTRL2 0x0E
#define VS1053_SCI_AICTRL3 0x0F


#define VS1053_CONTROL_SPI_SETTING  SPISettings(250000,  MSBFIRST, SPI_MODE0)
#define VS1053_DATA_SPI_SETTING     SPISettings(8000000, MSBFIRST, SPI_MODE0)

// END VS1053 Definitions


boolean AudioManagerClass::init()
{
	_isPlayingAudio = false;
	reset();
	delay(100);
	uint8_t vs1053begin = (sciRead(VS1053_REG_STATUS) >> 4) & 0x0F;

	// Dump on init
	// dumpRegs();
	return (vs1053begin == 4);
}

void AudioManagerClass::dumpRegs(void) {
	Serial.begin(115200, SERIAL_8N1);
	Serial.print("Mode = 0x"); Serial.println(sciRead(VS1053_REG_MODE), HEX);
	Serial.print("Stat = 0x"); Serial.println(sciRead(VS1053_REG_STATUS), HEX);
	Serial.print("ClkF = 0x"); Serial.println(sciRead(VS1053_REG_CLOCKF), HEX);
	Serial.print("Vol. = 0x"); Serial.println(sciRead(VS1053_REG_VOLUME), HEX);
	Serial.end();
}

void AudioManagerClass::reset()
{
	digitalWrite(Hardware.ampEnable, LOW);
	_ampEnabled = false;
	digitalWrite(Hardware.vs1053_reset, LOW);
	delay(100);
	digitalWrite(Hardware.vs1053_reset, HIGH);
	digitalWrite(Hardware.vs1053_cs, HIGH);
	digitalWrite(Hardware.vs1053_dcs, HIGH);
	delay(100);
	softReset();
	delay(100);

	sciWrite(VS1053_REG_CLOCKF, 0x6000);
}

void AudioManagerClass::softReset(void) {
	sciWrite(VS1053_REG_MODE, VS1053_MODE_SM_SDINEW | VS1053_MODE_SM_RESET);
	delay(100);
}

void AudioManagerClass::stopAudio()
{
	if (!_isPlayingAudio) 
	{
		return;
	}

	// cancel all playback
	sciWrite(VS1053_REG_MODE, VS1053_MODE_SM_LINE1 | VS1053_MODE_SM_SDINEW | VS1053_MODE_SM_CANCEL);

	// Turn off amp
	digitalWrite(Hardware.ampEnable, LOW);
	_ampEnabled = false;

	// Close the Current State
	_isPlayingAudio = false;
	//currentTrack.close();
}

void AudioManagerClass::spiwrite(uint8_t c)
{
	SPI.transfer(c);
}

void AudioManagerClass::sciWrite(uint8_t addr, uint16_t data) {
	SPI.beginTransaction(VS1053_CONTROL_SPI_SETTING);
	digitalWrite(Hardware.vs1053_cs, LOW);
	spiwrite(VS1053_SCI_WRITE);
	spiwrite(addr);
	spiwrite(data >> 8);
	spiwrite(data & 0xFF);
	digitalWrite(Hardware.vs1053_cs, HIGH);
	SPI.endTransaction();
}

uint16_t AudioManagerClass::sciRead(uint8_t addr) {
	uint16_t data;
	SPI.beginTransaction(VS1053_CONTROL_SPI_SETTING);
	digitalWrite(Hardware.vs1053_cs, LOW);
	spiwrite(VS1053_SCI_READ);
	spiwrite(addr);
	delayMicroseconds(10);
	data = spiread();
	data <<= 8;
	data |= spiread();
	digitalWrite(Hardware.vs1053_cs, HIGH);
	SPI.endTransaction();

	return data;
}

uint8_t AudioManagerClass::spiread(void)
{
	return SPI.transfer(0x00);
}

boolean AudioManagerClass::startAudioFile(const char * fileName)
{
	// reset current playback if any
	sciWrite(VS1053_REG_MODE, VS1053_MODE_SM_LINE1 | VS1053_MODE_SM_SDINEW);
	// resync
	sciWrite(VS1053_REG_WRAMADDR, 0x1e29);
	sciWrite(VS1053_REG_WRAM, 0);

	_currentTrack = SD.open(fileName);
	if (!_currentTrack) {
		_isPlayingAudio = false;
		return false;
	}

	// As explained in datasheet, set twice 0 in REG_DECODETIME to set time back to 0
	sciWrite(VS1053_REG_DECODETIME, 0x00);
	sciWrite(VS1053_REG_DECODETIME, 0x00);

	_isPlayingAudio = true;

	digitalWrite(Hardware.ampEnable, HIGH);
	_ampEnabled = true;
	// wait till its ready for data
	while (!readyForAudioData());

	// fill it up!
	while (_isPlayingAudio && readyForAudioData())
		feedAudioBuffer();

	return true;
}

boolean AudioManagerClass::playFullAudioFile(const char *trackname)
{
	if (!startAudioFile(trackname)) return false;

	while (_isPlayingAudio) {
		feedAudioBuffer();
	}
	// music file finished!
	return true;

}

boolean AudioManagerClass::readyForAudioData()
{
	return digitalRead(Hardware.vs1053_dreq);
}

void AudioManagerClass::feedAudioBuffer()
{
	static boolean running = false;

	if (!_isPlayingAudio || running)
	{
		return;
	}

	running = true;

	if (!_isPlayingAudio) {
		running = false;
		return; // paused or stopped
	}
	if (!_currentTrack) {
		running = false;
		return;
	}
	if (!readyForAudioData()) {
		running = false;
		return;
	}

	// Feed the hungry buffer! :)
	while (readyForAudioData()) {
		//UDR0 = '.';

		// Read some audio data from the SD card file
		int bytesread = _currentTrack.read(_mp3buffer, VS1053_DATABUFFERLEN);

		if (bytesread == 0) {
			// must be at the end of the file, wrap it up!
			_isPlayingAudio = false;
			_currentTrack.close();
			running = false;
			return;
		}
		playAudioData(_mp3buffer, bytesread);
		_lastAudioFeedTime = millis();
	}
	running = false;
	return;
}

void AudioManagerClass::playAudioData(uint8_t *buffer, uint8_t buffsiz) {
	SPI.beginTransaction(VS1053_DATA_SPI_SETTING);

	digitalWrite(Hardware.vs1053_dcs, LOW);
	for (uint8_t i = 0; i<buffsiz; i++) {
		spiwrite(buffer[i]);
	}
	digitalWrite(Hardware.vs1053_dcs, HIGH);
	SPI.endTransaction();
}

void AudioManagerClass::setAmpGain(uint8_t level)
{
	switch (level)
	{
	case 0:
		digitalWrite(Hardware.ampGain0, LOW);
		digitalWrite(Hardware.ampGain1, LOW);
		break;
	case 1:
		digitalWrite(Hardware.ampGain0, HIGH);
		digitalWrite(Hardware.ampGain1, LOW);
		break;
	case 2:
		digitalWrite(Hardware.ampGain0, LOW);
		digitalWrite(Hardware.ampGain1, HIGH);
		break;
	case 3:
		digitalWrite(Hardware.ampGain0, HIGH);
		digitalWrite(Hardware.ampGain1, HIGH);
		break;
	}
}

void AudioManagerClass::setVolume(uint8_t left, uint8_t right)
{
	uint16_t v;
	v = left;
	v <<= 8;
	v |= right;

	sciWrite(VS1053_REG_VOLUME, v);
}

void AudioManagerClass::updateAudioState()
{ 
	feedAudioBuffer(); 

	if (!_isPlayingAudio && _ampEnabled && ( (millis() - _lastAudioFeedTime) > 2000))
	{
		digitalWrite(Hardware.ampEnable, LOW);
		_ampEnabled = false;
	}
}

