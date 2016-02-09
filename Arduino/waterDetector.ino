void waterDetector(int interruptPin){
	// Use:
	// attachInterrupt(pin,function,mode)
	// Modes: LOW, HIGH, CHANGE, RISING, FALLING
	attachInterrupt(digitalPinToInterrupt(interruptPin), waterAlarm, RISING);
}

void waterAlarm(){
	// Send alarm to serial monitor for debugging:
	Serial.println("Water alarm!");
	// Send a SMS to boatowner:
	//sendSMS(phoneNumber, "Vann på båten! - Båtvakten");
	// Log the alarm on the server:
}

