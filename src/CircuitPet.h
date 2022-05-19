#ifndef CIRCUITPET_LIBRARY_CIRCUITPET_H
#define CIRCUITPET_LIBRARY_CIRCUITPET_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Loop/LoopListener.h>
#include "Pins.hpp"
#include <Display/Display.h>
#include <Input/InputGPIO.h>

class CircuitPetImpl {
public:
	CircuitPetImpl();

	void begin();

	const InputGPIO& getInput() const;
	const Display& getDisplay() const;

private:
	Display display;
	InputGPIO input;
};

extern CircuitPetImpl CircuitPet;


#endif //CIRCUITPET_LIBRARY_CIRCUITPET_H
