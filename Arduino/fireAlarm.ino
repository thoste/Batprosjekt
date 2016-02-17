// Fire alarm

void fireAlarm(){
	while(digitalRead(smokeDetectorPin) == HIGH){
		fireAlarmState = true;
		Serial.println("Fire alarm!");
		if(sendtFireAlarmSMS != true){
			Serial.println("Sending fire alarm SMS!");
			sendtFireAlarmSMS = true;
			minuteFireAlarmStart = minute();
			minuteFireAlarmStopp = minuteFireAlarmStart + 1;
			minutesSinceFireAlarm = minuteFireAlarmStart;
			tmpFireA = minuteFireAlarmStart;
		}
	}
	if(fireAlarmState == true){
		timeCounterFire();
	}	
}

