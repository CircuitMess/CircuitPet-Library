#ifndef CIRCUITPET_LIBRARY_BATTERYSERVICE_H
#define CIRCUITPET_LIBRARY_BATTERYSERVICE_H

#include <Arduino.h>
#include <Loop/LoopListener.h>

#define MAX_VOLT 4200
#define MIN_VOLT 3600


class BatteryService : private LoopListener {
public:
	BatteryService() = default;

	void loop(uint micros) override;
	void begin();
	uint8_t getLevel() const;
	uint8_t getPercentage() const;
	uint16_t getVoltage() const;

	static uint16_t mapReading(uint16_t reading);

	static int16_t getVoltOffset();

	bool charging() const;

private:
	uint16_t voltage = 0; //in mV
	static const uint16_t MeasureInterval;
	static const uint16_t MeasureCount;
	uint measureMicros = 0;
	float measureSum = 0;
	uint8_t measureCounter = 0;

};

extern BatteryService Battery;

#endif //CIRCUITPET_LIBRARY_BATTERYSERVICE_H
