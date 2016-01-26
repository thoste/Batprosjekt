void smokeDetector(){
	// Declare at the beginning of the main program: 
	//
	// const int  smokeDetectorPin = 2;
	// int smokeDetectorCounter = 0;   
	// int smokeDetectorState = 0;   
	// int lastSmokeDetectorState = 0; 


	// Declare in the void setup():
	//
	// pinMode(smokeDetectorPin, INPUT);
	// Serial.begin(57600);


	smokeDetectorState = digitalRead(smokeDetectorPin);
	if (smokeDetectorState != lastSmokeDetectorState) {
    	if (smokeDetectorState == HIGH) {
    		smokeDetectorCounter++;
      		Serial.println("Fire alarm is on!");
      		Serial.print("Number of firealarms:  ");
      		Serial.println(smokeDetectorCounter);
    	} 
    	else {
   			Serial.println("Fire alarm is off.");
   		}
    delay(50);
  	}
  	lastSmokeDetectorState = smokeDetectorState;
}