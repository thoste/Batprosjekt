void smokeDetectorInterrupt(int interruptPin){
	// attachInterrupt(pin,function,mode)
	// Modes: LOW, HIGH, CHANGE, RISING, FALLING
	attachInterrupt(digitalPinToInterrupt(interruptPin), smokeAlarm, RISING);
}

void smokeAlarm(){
	// Declare at the beginning of main:
	// volatile bool smokeAlarmState = false;
	smokeAlarmState = true;
}

void smokeAlarmOff(){
	if (smokeAlarmState == true){
		smokeCounter += 1;
		if(smokeCounter == 10){
			smokeCounter = 0;
			smokeAlarmState = false;
		}
	}
}