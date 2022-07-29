#ifndef CHATTER_LIBRARY_SETTINGS_H
#define CHATTER_LIBRARY_SETTINGS_H

#include <Arduino.h>

#define SHUTDOWN_STEPS 5

extern const uint32_t ShutdownSeconds[SHUTDOWN_STEPS];
extern const char* ShutdownText[SHUTDOWN_STEPS];

struct SettingsData {
	uint32_t shutdownTime = 4;
	uint8_t screenBrightness = 200;
	bool sound = true;
	bool RGBenable = true;
	bool hwTested = false;
};

class SettingsImpl {
public:
	bool begin();

	void store();

	SettingsData& get();

	void reset();

private:
	SettingsData data;
};

extern SettingsImpl Settings;

#endif //CHATTER_LIBRARY_SETTINGS_H
