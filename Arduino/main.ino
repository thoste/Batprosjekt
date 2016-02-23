#include <Arduino.h>
#include "Time.h"
#include "HardwareLink3.h"
#include "OneWire.h"
#include "DallasTemperature.h"



// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS_1 2
// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire Bus1(ONE_WIRE_BUS_1);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&Bus1);

// digital pins usable one Arduino Mega for interrupt: 2, 3, 18, 19
const int waterDetectorPin = 3;
const int smokeDetectorPin = 18;
const int smokeDetectorPin2 = 19;



void setup(){
	// Serial for debugging
	Serial.begin(9600);

	// Serial for modem
	//Serial3.begin(4600);

	// // Set up Unix time sync
	// unix_time_ts_sync = get_unix_ts();
	// millis_at_ts_sync = millis();
	// Serial.print("Time sync: ");
 	//	Serial.println(unix_time_ts_sync);

 	// hr, min, sec, day, month, year
 	setTime(15,35,00,10,2,2016);
	

	// Wateralarm pin interrupt
	pinMode(waterDetectorPin, INPUT);
	pinMode(smokeDetectorPin, INPUT);
	pinMode(smokeDetectorPin2, INPUT);
	//waterDetector(waterDetectorPin);

	// Firealarm pin interrupt
	// pinMode(smokeDetectorPin, INPUT);
	// smokeDetector(smokeDetectorPin);
	// pinMode(smokeDetectorPin2, INPUT);
	// smokeDetector(smokeDetectorPin2);

	// Temp sensors
	sensors.begin();
}


void loop(){
	//tempSensors();
	waterAlarm();
	fireAlarm();
	//counterWaterAlarm = 0;
	//delay(2000);
	printDate();
	delay(1000);
}



