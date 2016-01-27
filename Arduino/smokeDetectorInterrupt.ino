void smokeDetectorInterrupt(int interruptPin){
	// attachInterrupt(pin,function,mode)
	// Modes: LOW, HIGH, CHANGE, RISING, FALLING
	attachInterrupt(digitalPinToInterrupt(interruptPin), smokeAlarm, CHANGE);
}

void smokeAlarm(){
	// Declare at the beginning of main:
	// bool smokeAlarmState = false;
	smokeAlarmState = true;
}