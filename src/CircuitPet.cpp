#include "CircuitPet.h"
#include <Loop/LoopManager.h>
#include "Pins.hpp"
#include <SPIFFS.h>
#include "Settings.h"
#include <Audio/Piezo.h>
#include <ctime>

CircuitPetImpl CircuitPet;
ChirpSystem Audio;
RGBController RGB;

CircuitPetImpl::CircuitPetImpl() : display(160, 128, -1, -3), rtc(I2C_BM8563_DEFAULT_ADDRESS, Wire){

}

void CircuitPetImpl::begin(bool backlight){

	analogSetAttenuation(ADC_6db);

	SPIFFS.begin();

	display.getTft()->setPanel(CircuitPetDisplay::panel1());
	display.begin();
	display.getTft()->setRotation(3);
	display.swapBytes(false);
	display.clear(TFT_BLACK);
	display.commit();

	Settings.begin();

	input.preregisterButtons({ BTN_A, BTN_B, BTN_LEFT, BTN_RIGHT });
	LoopManager::addListener(&input);

	led.begin();
	led.setColor(Pixel::Black);
	led.setBrightness(Settings.get().RGBbrightness);
	RGB.begin(&led);

	Piezo.begin(PIN_BUZZ);
	Piezo.noTone();

	pinMode(PIN_BL, OUTPUT);
	digitalWrite(PIN_BL, 1);

	// Init RTC
	Wire.begin(I2C_SDA, I2C_SCL);
	rtc.begin();

	//check rtc time, if outside unixtime or invalid, set to unixtime 0
	I2C_BM8563_DateTypeDef dateStruct;
	rtc.getDate(&dateStruct);
	if(dateStruct.year < 1970 || dateStruct.year >= 2038 ||
	   dateStruct.date < 1 || dateStruct.date > 31 ||
	   dateStruct.month < 1 || dateStruct.month > 12){
		setUnixTime(0);
	}


	if(backlight){
		fadeIn();
	}
}

Input* CircuitPetImpl::getInput(){
	return &input;
}

Display* CircuitPetImpl::getDisplay(){
	return &display;
}

void CircuitPetImpl::initPWM(){
	ledcSetup(6, 5000, 8);
	ledcAttachPin(PIN_BL, 6);
	pwmInited = true;
}

void CircuitPetImpl::deinitPWM(){
	ledcDetachPin(PIN_BL);
	digitalWrite(PIN_BL, HIGH);
	pwmInited = false;
}

void CircuitPetImpl::setBrightness(uint8_t brightness){
	if(!pwmInited){
		initPWM();
	}

	ledcWrite(6, mapDuty(brightness));
}

bool CircuitPetImpl::backlightPowered() const{
	return pwmInited;
}

void CircuitPetImpl::backlightOff(){
	deinitPWM();
}

uint8_t CircuitPetImpl::mapDuty(uint8_t brightness){
	return map(brightness, 0, 255, 240, 0);
}

void CircuitPetImpl::fadeOut(uint8_t d){
	if(!pwmInited){
		initPWM();
	}

	uint8_t dutyOn = mapDuty(Settings.get().screenBrightness);

	for(int i = 0; i <= 255; i++){
		uint8_t val = map(i, 0, 255, dutyOn, 255);
		ledcWrite(6, val);
		delay(d);
	}

	deinitPWM();
}

void CircuitPetImpl::fadeIn(uint8_t d){
	if(!pwmInited){
		initPWM();
	}

	uint8_t dutyOn = mapDuty(Settings.get().screenBrightness);

	for(int i = 0; i <= 255; i++){
		uint8_t val = map(i, 0, 255, 255, dutyOn);
		ledcWrite(6, val);
		delay(d);
	}
}

void CircuitPetImpl::setUnixTime(time_t unixtime){
	std::tm* t = std::localtime(&unixtime);
	I2C_BM8563_DateTypeDef dateStruct { (int8_t)t->tm_wday, (int8_t)(t->tm_mon + 1), (int8_t)t->tm_mday, (int16_t)(t->tm_year + 1900) };
	I2C_BM8563_TimeTypeDef timeStruct { (int8_t)t->tm_hour, (int8_t)t->tm_min, (int8_t)t->tm_sec };

	rtc.setDate(&dateStruct);
	rtc.setTime(&timeStruct);
}

time_t CircuitPetImpl::getUnixTime(){

	I2C_BM8563_DateTypeDef dateStruct;
	I2C_BM8563_TimeTypeDef timeStruct;

	// Get RTC
	rtc.getDate(&dateStruct);
	rtc.getTime(&timeStruct);

	std::tm t { timeStruct.seconds,
				timeStruct.minutes,
				timeStruct.hours,
				dateStruct.date,
				dateStruct.month - 1,
				dateStruct.year - 1900};


	std::time_t time_stamp = mktime(&t);
	return time_stamp;
}
