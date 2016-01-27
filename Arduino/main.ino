// The main program 

const int smokeDetectorPin = 2;
bool smokeAlarmState = false;

void setup(){
	pinMode(smokeDetectorPin, INPUT);
	smokeDetectorInterrupt(smokeDetectorPin);
	Serial.begin(57600);
}

void loop(){
	Serial.println(smokeAlarmState);
	delay(100);
}