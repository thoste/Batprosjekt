void timeCounterWater(){
	if(minutesSinceWaterAlarm >= minuteWaterAlarmStopp){	
		waterAlarmState = false;
		Serial.println("Water alarm reset");
	}
	tmp_WaterB = minute();
	if(tmp_WaterA != tmp_WaterB){
		minutesSinceWaterAlarm++;
		tmp_WaterA = minute();
	}
}

void timeCounterFire(){
	if(minutesSinceFireAlarm >= minuteFireAlarmStopp){	
		fireAlarmState = false;
		Serial.println("Fire alarm reset");
	}
	tmp_FireB = minute();
	if(tmp_FireA != tmp_FireB){
		minutesSinceFireAlarm++;
		tmp_FireA = minute();
	}
}