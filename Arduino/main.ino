#include <Arduino.h>
#include "Time.h"
#include "HardwareLink3.h"
#include "DallasTemperature.h"

//TEMP
#define ONE_WIRE_BUS 2
OneWire Bus(ONE_WIRE_BUS);
DallasTemperature sensors(&Bus);

// Phone number of the boat owner
char* phoneNumber = "93266881";

//MODEM
byte* IMEI_nr = {};                           //Array that holds the IMEI number of the modem.

//TIMESTAMP
unsigned long ts_synced = 0;                  //Synchronized unix time stamp.
unsigned long millis_at_ts_sync = 0;          //millis() at the time of unix time synchronization.
unsigned long ts_now = 0;                     //Unix time stamp right now. Calculated with ts_synced and millis().
unsigned long millis_now = 0;                 //millis() right now.
byte ts_bytes[4] = {};                        //Array holding a time stamp (a long variable) that is cast to an array of 4 bytes.
bool ts_is_added_this_loop = false;           //Keeps track of if time stamp is added to the current row or not.
bool first_loop = true;                       //Is set to false at the end of the very first loop.

//DATA
byte data[2048] = {};                         //The array of bytes we send to the server.
long data_counter = 15;                       //Number of bytes in the data array. It is initialized to 15 because we already added the IMEI number digit by digit.                                         
long data_counter_start;                      //Data counter at start of loop.
long data_send_interval = 30000;              //The time between transfers of data to the server.
unsigned long millis_at_last_send = 0;        //millis() at last transfer.
char* ip_adr;                                 //C-string to hold the address we are assigned from the telecom operator.
char ping_adr[32] = "www.google.com";         //The address we wish to ping.

//TEMP
int iTemp = 0;
double temps1[11];
double temps2[11];
double temps3[11];

// digital pins usable one Arduino Mega for interrupt: 2, 3, 18, 19
const int waterDetectorPin = 3;
const int smokeDetectorPin = 18;
const int smokeDetectorPin2 = 19;

//FIRE
bool fireAlarmState = false;
bool sendtFireAlarmSMS = false;
int minuteFireAlarmStart = 0;
int minuteFireAlarmStopp = 0;
int minutesSinceFireAlarm = 0;
int tmp_FireA = 0;
int tmp_FireB = 0;

//WATER
bool waterAlarmState = false;
int counterWaterAlarm = 0;
int minuteWaterAlarmStart = 0;
int minuteWaterAlarmStopp = 0;
int minutesSinceWaterAlarm = 0;
int tmp_WaterA = 0;
int tmp_WaterB = 0;
//test

void setup(){
    // Serial used for debugging
    Serial.begin(9600);

    // Modem startup
    Serial3.begin(4800);                            //We use Serial3 for communication with the modem.
    Serial.println(F("Modem booting"));
    modemStart();    // Boot modem and enter PIN    
    Serial.println(F("Modem boot completed"));      
    Serial.println(F("Entering modem setup"));
    if(GPRS_setup()){                               //Configures the modem so that it is able to send data and SMS.
        Serial.println(F("Modem setup completed"));   
    }
    else{
        Serial.println(F("Modem setup failed"));
    }
    ip_adr = get_IP();                              //Holding the IP address given to us by the telecom operator.
    Serial.print("IP address: ");
    Serial.println(ip_adr);
    IMEI_nr = get_IMEI_nr();                        //Holding the IMEI number of the modem.
    for(int i = 0; i < 15; i++){                    //Adding the IMEI number to data.
        data[i] = IMEI_nr[i];
    }
    Serial.print("IMEI number: ");
    for(int i = 0; i < 15; i++){
        Serial.print((char)data[i]);
    }
    Serial.println(" ");
    //Time sync
    ts_synced = get_unix_ts();                      //Synchronizing time
    millis_at_ts_sync = millis();                   //Saving millis() at time of synchronization
    Serial.print("Time sync: ");
    Serial.println(ts_synced);
    Serial.print("Signal strenght: ");
    Serial.println(getSignalStrength());
    if(GPRS_ping(ping_adr)){                        //Pinging the Internet.
        Serial.print("Pinged "); 
        Serial.print(ping_adr);
        Serial.println(" successfully");
    }
    else{
        Serial.print("ERROR: Could not ping ");
        Serial.println(ping_adr);
    }
    //Resetting time intervals
    millis_now = millis();
    millis_at_last_send = millis_now;

    // Temp sensors begin
    sensors.begin();

    // Fire and water sensor pins
    pinMode(waterDetectorPin, INPUT);
    pinMode(smokeDetectorPin, INPUT);
    pinMode(smokeDetectorPin2, INPUT);

    //sendSMS(phoneNumber, "Arduinbo bootup complete!");
}

void loop(){
    Serial.print("ts_synced. ");
    Serial.println(ts_synced);
    Serial.print("Minute: ");
    Serial.println(minute());
    Serial.print("From GSM: ");
    Serial.println(Serial3.print(F("AT+CNTP")));

	// tempSensors();
 //    waterAlarm();
 //    fireAlarm();
	delay(1000);
}

void fireAlarm(){
	while(digitalRead(smokeDetectorPin) == HIGH){
		fireAlarmState = true;
		Serial.println("Fire alarm!");
		if(sendtFireAlarmSMS != true){
			Serial.println("Sending fire alarm SMS!");
			// Send SMS;
            //sendSMS(phoneNumber, "Det brenner på båten!");
			sendtFireAlarmSMS = true;
			minuteFireAlarmStart = minute();
			minuteFireAlarmStopp = minuteFireAlarmStart + 1;
			minutesSinceFireAlarm = minuteFireAlarmStart;
			tmp_FireA = minuteFireAlarmStart;
			break;
		}
	}
	if(fireAlarmState == true){
		timeCounterFire();
	}	
}

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
            //sendSMS("Det er vann på båten!");
			counterWaterAlarm = 0;
			minuteWaterAlarmStart = minute();
			minuteWaterAlarmStopp = minuteWaterAlarmStart + 1;
			minutesSinceWaterAlarm = minuteWaterAlarmStart;
			tmp_WaterA = minuteWaterAlarmStart;
			break;
		}
	}
	if(waterAlarmState == true){
		timeCounterWater();
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

void tempSensors(){
	sensors.requestTemperatures();
    temps1[iTemp] = sensors.getTempCByIndex(0);
    temps2[iTemp] = sensors.getTempCByIndex(1);
	temps3[iTemp] = sensors.getTempCByIndex(2);
    iTemp++;
    if (iTemp == 11){
        Serial.println(middle(temps1, 11));
        Serial.println(middle(temps2, 11));
		Serial.println(middle(temps3, 11));
        iTemp = 0;
    }
}

double middle(double arr[], int size) {
    double* temp = new double[size];
    for (int i = 0; i < size; i++) {
        temp[i] = arr[i];
    }
    insSort(temp, size);
    return temp[size / 2];
    delete[] temp;
}

void insSort(double arr[], int size) {
    int j, temp; 
    for (int i = 0; i < size; i++) {
        j = i;
        while ((j > 0) && (arr[j] < arr[j - 1])) {
            temp = arr[j];
            arr[j] = arr[j - 1];
            arr[j - 1] = temp;
            j--;
        }
    }
}
