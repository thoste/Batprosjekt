#include "include.h"


void setup(){
	// Serial for debugging
	Serial.begin(9600);

	// Serial for modem
	Serial3.begin(4600);

	// Firealarm pin interrupt
	pinMode(smokeDetectorPin, INPUT);
	smokeDetectorInterrupt(smokeDetectorPin);
	pinMode(smokeDetectorPin2, INPUT);
	smokeDetectorInterrupt(smokeDetectorPin2);
}

void loop(){
}