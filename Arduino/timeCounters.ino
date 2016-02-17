void timeCounterWater(){
	if(minutesSinceWaterAlarm >= minuteWaterAlarmStopp){	
		waterAlarmState = false;
		Serial.println("Water alarm reset");
	}
	tempWaterB = minute();
	if(tempWaterA != tempWaterB){
		minutesSinceWaterAlarm++;
		tempWaterA = minute();
	}
}

void timeCounterFire(){
	if(minutesSinceFireAlarm >= minuteFireAlarmStopp){	
		fireAlarmState = false;
		Serial.println("Fire alarm reset");
	}
	tempFireB = minute();
	if(tempFireA != tempFireB){
		minutesSinceFireAlarm++;
		tempFireA = minute();
	}
}