#include <Arduino.h>
#include <Time.h>
#include <HardwareLink3.h>

// Phone number of the boat owner
char* phoneNumber = "95174794";

// Smoke detetctor pin
// digital pins usable for interrupt 2, 3, 18, 19, 20, 21
const int smokeDetectorPin = 2;
const int smokeDetectorPin2 = 2;



void setup(){
	// Serial for debugging
	Serial.begin(9600);

	// Serial for modem
	Serial3.begin(4800);

	// Firealarm pin interrupt
	pinMode(smokeDetectorPin, INPUT);
	smokeDetectorInterrupt(smokeDetectorPin);
	pinMode(smokeDetectorPin2, INPUT);
	smokeDetectorInterrupt(smokeDetectorPin2);
}

void loop(){
}