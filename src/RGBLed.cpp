#include "RGBLed.h"
#include "Pins.hpp"
#include <Arduino.h>

RGBLed RGB;

void RGBLed::begin(){
	pinMode(RGB_R, OUTPUT);
	pinMode(RGB_G, OUTPUT);
	pinMode(RGB_B, OUTPUT);

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
	ledcWrite(0, 255 - color.r);
	ledcWrite(1, 255 - color.g);
	ledcWrite(2, 255 - color.b);
}
