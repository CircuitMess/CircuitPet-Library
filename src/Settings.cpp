#include "Settings.h"
#include <Util/Settings.h>

const uint32_t ShutdownSeconds[SHUTDOWN_STEPS] = { 0, 30, 60, 2 * 60, 5 * 60 };
const char* ShutdownText[SHUTDOWN_STEPS] = { "OFF", "30 sec", "1 min", "2 min", "5 min" };


SettingsImpl Settings;

bool SettingsImpl::begin(){
	return Settings::init(&data, sizeof(SettingsData));
}

SettingsData& SettingsImpl::get(){
	return data;
}

void SettingsImpl::store(){
	Settings::store();
}

void SettingsImpl::reset(){
	data = SettingsData{};
	SettingsImpl::store();
}