// Water alarm

void waterAlarm(){
	while(digitalRead(waterDetectorPin) == HIGH && waterAlarmState != true){
		counterWaterAlarm += 1;
		delay(1000);
		// Needs to be HIGH for X seconds to raise an alarm
		if (counterWaterAlarm == 10){
			waterAlarmState = true;
			Serial.println("WATER ALARM!");
			Serial.println("Sending SMS to owner.");
			// Send SMS
			counterWaterAlarm = 0;
			minuteWaterAlarmStart = minute();
			minuteWaterAlarmStopp = minuteWaterAlarmStart + 1;
			minutesSinceWaterAlarm = minuteWaterAlarmStart;
			tmpA = minuteWaterAlarmStart;
			break;
		}
	}
	if(waterAlarmState == true){
		timeCounterWater();
	}
	
}

void timeCounterWater(){
	if(minutesSinceWaterAlarm >= minuteWaterAlarmStopp){	
		waterAlarmState = false;
		Serial.println("Water alarm reset");
	}
	tmpB = minute();
	if(tmpA != tmpB){
		minutesSinceWaterAlarm++;
		tmpA = minute();
	}
}