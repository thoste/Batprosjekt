void waterAlarm(){
	while(digitalRead(waterDetectorPin) == HIGH){
		n += 1;
		delay(1000);
		if (n == 5){
			waterCounter += 1;
			Serial.println("Water!");
			if(waterCounter == 3 && waterSensorAlarm != true){
				waterSensorAlarm = true;
				Serial.println("WAAAATHAAA!");
				// Send SMS
				waterCounter = 0;
			}
			break;
		}
	}
}

void timeCounterWater(){
	if(waterSensorAlarm == true){
		timeCounterWaterAlarm += 1;
		if(timeCounterWaterAlarm >= 10000){
			// Reset the waterAlarm after some time
			waterSensorAlarm = false;
		}
	}
}