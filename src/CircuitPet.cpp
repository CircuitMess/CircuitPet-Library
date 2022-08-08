#include "CircuitPet.h"
#include <Loop/LoopManager.h>
#include "Pins.hpp"
#include <SPIFFS.h>
#include "Settings.h"
#include <Audio/Piezo.h>

CircuitPetImpl CircuitPet;

CircuitPetImpl::CircuitPetImpl() : display(160, 128, -1, -3){

}

void CircuitPetImpl::begin(bool backlight){

	analogSetAttenuation(ADC_6db);

	SPIFFS.begin();

	display.begin();
	display.getTft()->setRotation(3);
	display.swapBytes(false);
	display.clear(TFT_BLACK);
	display.commit();

	Settings.begin();

	input.preregisterButtons({ BTN_A, BTN_B, BTN_C, BTN_D });
	LoopManager::addListener(&input);

	RGB.begin();
	RGB.setColor(Pixel::Black);

	Piezo.begin(PIN_BUZZ);
	Piezo.noTone();

	pinMode(PIN_BL, OUTPUT);
	digitalWrite(PIN_BL, 1);

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

void CircuitPetImpl::fadeOut(){
	if(!pwmInited){
		initPWM();
	}

	uint8_t dutyOn = mapDuty(Settings.get().screenBrightness);

	for(int i = 0; i <= 255; i++){
		uint8_t val = map(i, 0, 255, dutyOn, 255);
		ledcWrite(6, val);
		delay(2);
	}

	deinitPWM();
}

void CircuitPetImpl::fadeIn(){
	if(!pwmInited){
		initPWM();
	}

	uint8_t dutyOn = mapDuty(Settings.get().screenBrightness);

	for(int i = 0; i <= 255; i++){
		uint8_t val = map(i, 0, 255, 255, dutyOn);
		ledcWrite(6, val);
		delay(2);
	}
}
