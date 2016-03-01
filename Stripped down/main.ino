#include <Arduino.h>
#include "Time.h"
#include "HardwareLink3.h"


void setup(){
  // Serial used for debugging
  Serial.begin(9600);
  // Modem startup
  modemSetup();
}

void loop(){
	Serial.print("Signal strenght: ");
	Serial.println(getSignalStrength());
	delay(1000);
}