#ifndef CIRCUITPET_LIBRARY_CIRCUITPET_H
#define CIRCUITPET_LIBRARY_CIRCUITPET_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Loop/LoopListener.h>
#include "Pins.hpp"
#include <Display/Display.h>
#include <Input/InputGPIO.h>
#include "RGBLed.h"

class CircuitPetImpl {
public:
	CircuitPetImpl();

	void begin();

	InputGPIO& getInput();
	Display& getDisplay();

private:
	Display display;
	InputGPIO input;
};

extern CircuitPetImpl CircuitPet;


#endif //CIRCUITPET_LIBRARY_CIRCUITPET_H
