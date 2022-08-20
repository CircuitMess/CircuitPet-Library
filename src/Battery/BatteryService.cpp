#include "BatteryService.h"
#include <Loop/LoopManager.h>
#include <soc/efuse_reg.h>
#include "../Pins.hpp"
#include "../CircuitPet.h"

BatteryService Battery;

const uint16_t BatteryService::MeasureInterval = 10; //in seconds
const uint16_t BatteryService::MeasureCount = 10; //in seconds

uint16_t BatteryService::mapReading(uint16_t reading){
	return map(reading, 1600, 1950, MIN_VOLT, MAX_VOLT);
}

void BatteryService::loop(uint micros){
	measureMicros += micros;
	if(measureMicros >= (MeasureInterval * 1000000) / MeasureCount){
		measureMicros = 0;
		measureSum += analogRead(PIN_BATT);
		measureCounter++;
		if(measureCounter == MeasureCount){
			measureSum = measureSum / MeasureCount;
			voltage = mapReading(measureSum);
			measureCounter = 0;
			measureSum = 0;

			if(getVoltage() < MIN_VOLT && !charging()){
				CircuitPet.shutdown();
				return;
			}
		}
	}
}

void BatteryService::begin(){
	LoopManager::addListener(this);
	pinMode(PIN_BATT, INPUT);
	for(int i = 0; i < 10; i++){
		measureSum += analogRead(PIN_BATT);
	}
	measureSum = measureSum / MeasureCount;
	voltage = mapReading(measureSum);
	measureSum = 0;

	if(getVoltage() < MIN_VOLT && !charging()){
		CircuitPet.shutdown();
		return;
	}
}

uint8_t BatteryService::getLevel() const{
	uint8_t percentage = getPercentage();
	if(percentage > 80){
		return 5;
	}else if(percentage > 60){
		return 4;
	}else if(percentage > 40){
		return 3;
	}else if(percentage > 20){
		return 2;
	}else if(percentage >= 5){
		return 1;
	}else if(percentage < 5){
		return 0;
	}
}

uint8_t BatteryService::getPercentage() const{
	int16_t percentage = map(getVoltage(), MIN_VOLT, MAX_VOLT, 0, 100);
	if(percentage < 0){
		return 0;
	}else if(percentage > 100){
		return 100;
	}else{
		return percentage;
	}
}

uint16_t BatteryService::getVoltage() const{
	return voltage + getVoltOffset();
}

int16_t BatteryService::getVoltOffset(){
	uint32_t upper = REG_GET_FIELD(EFUSE_BLK3_RDATA3_REG, EFUSE_RD_ADC1_TP_HIGH);
	uint32_t lower = REG_GET_FIELD(EFUSE_BLK3_RDATA3_REG, EFUSE_RD_ADC1_TP_LOW);
	return (upper << 7) | lower;
}

bool BatteryService::charging() const{
	return digitalRead(PIN_CHARGE);
}