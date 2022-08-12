#include "RGBLed.h"
#include "Pins.hpp"
#include <Arduino.h>

RGBLed RGB;

void RGBLed::begin(){
	pinMode(RGB_R, OUTPUT);
	pinMode(RGB_G, OUTPUT);
	pinMode(RGB_B, OUTPUT);
	digitalWrite(RGB_R, HIGH);
	digitalWrite(RGB_G, HIGH);
	digitalWrite(RGB_B, HIGH);

	ledcSetup(0, 5000, 8);
	ledcSetup(1, 5000, 8);
	ledcSetup(2, 5000, 8);

	ledcAttachPin(RGB_R, 0);
	ledcAttachPin(RGB_G, 1);
	ledcAttachPin(RGB_B, 2);
	attached = true;
}

const Pixel& RGBLed::getColor() const{
	return color;
}

void RGBLed::setColor(const Pixel& color){

	if(color == Pixel::Black){
		ledcDetachPin(RGB_R);
		ledcDetachPin(RGB_G);
		ledcDetachPin(RGB_B);

		pinMode(RGB_R, OUTPUT);
		pinMode(RGB_G, OUTPUT);
		pinMode(RGB_B, OUTPUT);

		digitalWrite(RGB_R, HIGH);
		digitalWrite(RGB_G, HIGH);
		digitalWrite(RGB_B, HIGH);

		attached = false;
	}else if(!attached){
		attached = true;
		ledcAttachPin(RGB_R, 0);
		ledcAttachPin(RGB_G, 1);
		ledcAttachPin(RGB_B, 2);
	}
	RGBLed::color = color;

	double mapped = pow((brightness / 255.0), 2);
	ledcWrite(0, (255 - mapped*color.r));
	ledcWrite(1, (255 - mapped*color.g));
	ledcWrite(2, (255 - mapped*color.b));
}

uint8_t RGBLed::getBrightness() const{
	return brightness;
}

void RGBLed::setBrightness(uint8_t brightness){
	RGBLed::brightness = brightness;
}
