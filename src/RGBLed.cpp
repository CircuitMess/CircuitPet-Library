#include "RGBLed.h"
#include "Pins.hpp"
#include <Arduino.h>

RGBLed RGB;

void RGBLed::begin(){
	ledcSetup(0, 5000, 8);
	ledcSetup(1, 5000, 8);
	ledcSetup(2, 5000, 8);

	ledcAttachPin(RGB_R, 0);
	ledcAttachPin(RGB_G, 1);
	ledcAttachPin(RGB_B, 2);
}

const Pixel& RGBLed::getColor() const{
	return color;
}

void RGBLed::setColor(const Pixel& color){
	RGBLed::color = color;
	ledcWrite(RGB_R, color.r);
	ledcWrite(RGB_G, color.g);
	ledcWrite(RGB_B, color.b);
}
