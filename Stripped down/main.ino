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
  millis_now = millis();
}