// The main program 


const int  smokeDetectorPin = 2;
int smokeDetectorCounter = 0;   
int smokeDetectorState = 0;   
int lastSmokeDetectorState = 0; 

void setup(){
	pinMode(smokeDetectorPin, INPUT);
	Serial.begin(57600);
}

void loop(){
	smokeDetector();
}