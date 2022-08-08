#ifndef CIRCUITPET_LIBRARY_RGBLED_H
#define CIRCUITPET_LIBRARY_RGBLED_H
#include <Display/Color.h>

class RGBLed {
public:
	void begin();
	const Pixel& getColor() const;
	void setColor(const Pixel& color);

private:
	Pixel color = {0, 0, 0};
	bool attached = false;
};

extern RGBLed RGB;


#endif //CIRCUITPET_LIBRARY_RGBLED_H
