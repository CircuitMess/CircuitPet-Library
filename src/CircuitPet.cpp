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

	LoopManager::addListener(&input);

}

const InputGPIO& CircuitPetImpl::getInput() const{
	return input;
}

const Display& CircuitPetImpl::getDisplay() const{
	return display;
}


