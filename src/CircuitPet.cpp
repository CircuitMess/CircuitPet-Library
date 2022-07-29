#include "CircuitPet.h"
#include <Loop/LoopManager.h>
#include "Pins.hpp"
#include <SPIFFS.h>

CircuitPetImpl CircuitPet;

CircuitPetImpl::CircuitPetImpl() : display(160, 128, -1, -1){

}

void CircuitPetImpl::begin(){

	SPIFFS.begin();

	display.begin();
	display.clear(TFT_BLACK);
	display.commit();

	RGB.begin();

	LoopManager::addListener(&input);

}

InputGPIO& CircuitPetImpl::getInput(){
	return input;
}

Display& CircuitPetImpl::getDisplay(){
	return display;
}


