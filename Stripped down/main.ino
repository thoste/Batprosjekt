#include <Arduino.h>
#include "Time.h"
#include "HardwareLink3.h"


unsigned long unixTime = 1457526789;

void setup(){
  // Serial used for debugging
  Serial.begin(9600);
  // Modem startup
  //modemSetup();
  //setTime(unixTime);
}

void loop(){
	// Serial.print("Hour: ");
	// Serial.println(hour());
	// Serial.print("Minute: ");
	// Serial.println(minute());
	// Serial.print("Second: ");
	// Serial.println(second());
	// delay(1000);
}