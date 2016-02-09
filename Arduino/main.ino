#include <Arduino.h>
#include "Time.h"
#include "HardwareLink3.h"
#include "OneWire.h"
#include "DallasTemperature.h"

// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS_1 2

// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire Bus1(ONE_WIRE_BUS_1);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&Bus1);
double temp1;
double temp2;
double tempM;


// Phone number of the boat owner
char* phoneNumber = "95174794";


// digital pins usable one Arduino Mega for interrupt: 2, 3, 18, 19
const int waterDetectorPin = 3;
const int smokeDetectorPin = 18;
const int smokeDetectorPin2 = 19;



// //MODEM
// long pincode = 0000;                          //The PIN code of the SIM card inserted in the modem.
// byte* IMEI_nr = {};                           //Array that holds the IMEI number of the modem.

// //TIMESTAMP
// unsigned long ts_synced = 0;                  //Synchronized unix time stamp.
// unsigned long millis_at_ts_sync = 0;          //millis() at the time of unix time synchronization.
// unsigned long ts_now = 0;                     //Unix time stamp right now. Calculated with ts_synced and millis().
// unsigned long millis_now = 0;                 //millis() right now.
// byte ts_bytes[4] = {};                        //Array holding a time stamp (a long variable) that is cast to an array of 4 bytes.
// bool ts_is_added_this_loop = false;           //Keeps track of if time stamp is added to the current row or not.
// bool first_loop = true;                       //Is set to false at the end of the very first loop.

// //DATA
// byte data[2048] = {};                         //The array of bytes we send to the server.
// long data_counter = 15;                       //Number of bytes in the data array. 
//                                               //It is initialized to 15 because we already added the IMEI number digit by digit.   
// long data_counter_start;                      //Data counter at start of loop.
// long data_send_interval = 30000;              //The time between transfers of data to the server.
// unsigned long millis_at_last_send = 0;        //millis() at last transfer.
// char* ip_adr;                                 //C-string to hold the address we are assigned from the telecom operator.
// char ping_adr[32] = "www.google.com";         //The address we wish to ping.



void setup(){
	// Serial for debugging
	Serial.begin(9600);

	// Serial for modem
	//Serial3.begin(4600);

	// Wateralarm pin interrupt
	pinMode(waterDetectorPin, INPUT);
	//waterDetector(waterDetectorPin);

	// Firealarm pin interrupt
	// pinMode(smokeDetectorPin, INPUT);
	// smokeDetector(smokeDetectorPin);
	// pinMode(smokeDetectorPin2, INPUT);
	// smokeDetector(smokeDetectorPin2);

	// Temp sensors
	sensors.begin();
}

bool waterSensorAlarm = false;
long timeCounterWaterAlarm = 0;
int n = 0;
int waterCounter = 0;


void loop(){
	tempSensors();
	waterAlarm();
	timeCounterWater();
	n = 0;
}



