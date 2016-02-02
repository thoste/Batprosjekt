// The main program 

const int smokeDetectorPin = 2;
volatile bool smokeAlarmState = false;
int smokeCounter = 0;

void setup(){
	pinMode(smokeDetectorPin, INPUT);
	smokeDetectorInterrupt(smokeDetectorPin);
	Serial.begin(57600);
}

void loop(){
	Serial.println(smokeAlarmState);
	if(smokeAlarmState == true){
		smokeAlarmOff();
	}
	delay(1000);
}