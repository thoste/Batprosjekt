void smokeDetectorInterrupt(int interruptPin){
	// Use:
	// attachInterrupt(pin,function,mode)
	// Modes: LOW, HIGH, CHANGE, RISING, FALLING
	attachInterrupt(digitalPinToInterrupt(interruptPin), smokeAlarm, RISING);
}

void smokeAlarm(){
	// Send alarm to serial monitor for debugging:
	Serial.println("Fire alarm!");
	// Send a SMS to boatowner:
	sendSMS(phoneNumber, "Brannalarm på båten! - Båtvakten");
	// Log the alarm on the server:
}

