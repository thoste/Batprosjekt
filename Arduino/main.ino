#include <Arduino.h>
#include <string.h>
#include <cstring.h>
#include "Time.h"
#include "HardwareLink3.h"
#include "DallasTemperature.h"


//TEMP
#define ONE_WIRE_BUS 2
OneWire Bus(ONE_WIRE_BUS);
DallasTemperature sensors(&Bus);

// Phone number of the boat owner
char* phoneNumber = "95174794";

//MODEM
byte* IMEI_nr = {};                           //Array that holds the IMEI number of the modem.

//TIMESTAMP
unsigned long ts_synced = 0;                  //Synchronized unix time stamp.
unsigned long ts_now = 0;
byte ts_bytes[4] = {};                        //Array holding a time stamp (a long variable) that is cast to an array of 4 bytes.

//DATA
bool timeToSendData = false;
byte data[64] = {};                         //The array of bytes we send to the server.
long data_counter = 15;                       //Number of bytes in the data array. It is initialized to 15 because we already added the IMEI number digit by digit.                                         
char* ip_adr;                                 //C-string to hold the address we are assigned from the telecom operator.
char ping_adr[32] = "www.google.com";         //The address we wish to ping.
int numErrSend = 0;

//TEMP
bool tempSampleState = false;
int iTemp = 0;
double tempStore[3][3] = {0};
int minuteTempSampleStart = 0;
int minuteTempSampleStopp = 0;
int tmpTempA = 0;
int tmpTempB = 0;


//FIRE
const int fireAlarmPin[2] = {16,17}; //Sets the pin input on the arduino for the fire alarm pins
bool fireAlarmState[2] = {0};          //Holds the fire alarm boolean states - default is false
int minuteFireAlarmStart[2] = {0};
int minuteFireAlarmStopp[2] = {0};
int tmpFireA[2] = {0};
int tmpFireB[2] = {0};

//WATER
const int waterAlarmPin[6] = {3,4,5,10,11,12}; //Sets the pin input on the arduino for the water alarm pins
bool waterAlarmState[6] = {0};          //Holds the water alarm boolean states - default is false
int counterWaterAlarm[6] = {0};
int minuteWaterAlarmStart[6] = {0};
int minuteWaterAlarmStopp[6] = {0};
int tmpWaterA[6] = {0};
int tmpWaterB[6] = {0};

// LAND POWER
const int landPowerPin = 6;
bool powerAlarmState = false;
int counterPowerAlarm = 0;
int minutePowerAlarmStart = 0;
int minutePowerAlarmStopp = 0;
int tmpPowerA = 0;
int tmpPowerB = 0;


void setup(){
    // Serial used for debugging
    Serial.begin(9600);

    // Modem startup
    Serial3.begin(4800);                            //We use Serial3 for communication with the modem.
    Serial.println(F("Modem booting"));
    initModem();    // Boot modem and enter PIN    
    Serial.println(F("Modem boot completed"));      
    Serial.println(F("Entering modem setup"));

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
    setTime(ts_synced);                             //Sets local Arduino time to the server time
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

    // Temp sensors begin
    sensors.begin();

    // Fire sensor pins
    pinMode(fireAlarmPin[0], INPUT);
    pinMode(fireAlarmPin[1], INPUT);
    // Water sensor pins
    pinMode(waterAlarmPin[0], INPUT);
    pinMode(waterAlarmPin[1], INPUT);
    pinMode(waterAlarmPin[2], INPUT);
    pinMode(waterAlarmPin[3], INPUT);
    pinMode(waterAlarmPin[4], INPUT);
    pinMode(waterAlarmPin[5], INPUT);
    // Land power pin
    pinMode(landPowerPin, INPUT);

    delay(1000);

    // After setup its time to sample the temperatures
    tempSampleState = true;

    sendSMS(phoneNumber, "Arduinbo bootup complete!");
    Serial.println("Setup completed!");
}

void loop(){

	temperature();

    fireAlarm(0,fireAlarmPin[0]);
    fireAlarm(1,fireAlarmPin[1]);

    waterAlarm(0,waterAlarmPin[0]);
    waterAlarm(1,waterAlarmPin[1]);
    waterAlarm(2,waterAlarmPin[2]);
    waterAlarm(3,waterAlarmPin[3]);
    waterAlarm(4,waterAlarmPin[4]);
    waterAlarm(5,waterAlarmPin[5]);

    landPower(landPowerPin);

    sendDataToServer();
	delay(5000);
}

void fireAlarm(int alarmNumber, int alarmPin){
	while(digitalRead(alarmPin) == HIGH && fireAlarmState[alarmNumber] != true){
		Serial.print("Fire alarm for number: ");
        Serial.println(alarmNumber);
		sendAlarmSMS('F', alarmNumber);
		minuteFireAlarmStart[alarmNumber] = minute();
        tmpFireA[alarmNumber] = minute();
		minuteFireAlarmStopp[alarmNumber] = minuteFireAlarmStart[alarmNumber] + 10;
        fireAlarmState[alarmNumber] = true;
		break;
	}
	if(fireAlarmState[alarmNumber] == true){
		timeCounter(&minuteFireAlarmStart[alarmNumber], minuteFireAlarmStopp[alarmNumber], &tmpFireA[alarmNumber], &tmpFireB[alarmNumber], &fireAlarmState[alarmNumber]);
	}	
}

void waterAlarm(int alarmNumber, int alarmPin){
	while(digitalRead(alarmPin) == HIGH && waterAlarmState[alarmNumber] != true){
		counterWaterAlarm[alarmNumber] += 1;
		delay(1000);
		// Needs to be HIGH for X seconds to raise an alarm
		if (counterWaterAlarm[alarmNumber] == 10){
			Serial.print("Water alarm for number: ");
            Serial.println(alarmNumber);
			counterWaterAlarm[alarmNumber] = 0;
			minuteWaterAlarmStart[alarmNumber] = minute();
            tmpWaterA[alarmNumber] = minute();
			minuteWaterAlarmStopp[alarmNumber] = minuteWaterAlarmStart[alarmNumber] + 10;
            sendAlarmSMS('W', alarmNumber);
            waterAlarmState[alarmNumber] = true;
			break;
		}
	}
	if(waterAlarmState[alarmNumber] == true){
		timeCounter(&minuteWaterAlarmStart[alarmNumber], minuteWaterAlarmStopp[alarmNumber], &tmpWaterA[alarmNumber], &tmpWaterB[alarmNumber], &waterAlarmState[alarmNumber]);
	}
}

void landPower(int pin){
    while(digitalRead(pin) == LOW && powerAlarmState != true){
        counterPowerAlarm += 1;
        delay(1000);
        // Needs to be low for X seconds to raise an alarm
        if(counterPowerAlarm == 10){
            Serial.println("Land power alarm! No land power!");
            counterPowerAlarm = 0;
            minutePowerAlarmStart = minute();
            tmpPowerA = minute();
            minutePowerAlarmStopp = minutePowerAlarmStart + 10;
            sendAlarmSMS('P', 0);
            powerAlarmState = true;
        }
    }
    if(powerAlarmState == true){
        timeCounter(&minutePowerAlarmStart, minutePowerAlarmStopp, &tmpPowerA, &tmpPowerB, &powerAlarmState);
    }
}

void temperature(){
    while(tempSampleState == true){
        sensors.requestTemperatures();
        tempStore[0][iTemp] = sensors.getTempCByIndex(0);
        tempStore[1][iTemp] = sensors.getTempCByIndex(1);
        tempStore[2][iTemp] = sensors.getTempCByIndex(2);
        Serial.print("Temps: ");
        Serial.print(tempStore[0][iTemp]);
        Serial.print("\t");
        Serial.print(tempStore[1][iTemp]);
        Serial.print("\t");
        Serial.println(tempStore[2][iTemp]);
        if(iTemp == 2){
            Serial.println("Temp store done 3 times, time to send data to server");
            timeToSendData = true;
            iTemp = 0;
        }
        else{
            iTemp++;
        }
        minuteTempSampleStart = minute();
        tmpTempA = minute();
        minuteTempSampleStopp = minuteTempSampleStart + 1;
        tempSampleState = false;
    }
    if(tempSampleState != true){
        timeCounter(&minuteTempSampleStart, minuteTempSampleStopp, &tmpTempA, &tmpTempB, &tempSampleState);
    }
}

void sendDataToServer(){
    if(timeToSendData == true){
        // Adding timestamp to the data array
        ts_now = now();  // Getting the UNix time now from the Arduino
        for(int i = 0; i < 4; i++){
            data[data_counter] = (byte)((ts_now >> 8*(3 - i)) & 255);
            data_counter++;
        }

        //Adding the temperatures to the data array
        union {
            float temp;
            byte arr[4];
        };
        for (int i = 0; i < 3; i++){
            for (int j = 0; j < 3; j++){
                temp = tempStore[i][j];
                for (int k = 0; k < 4; k++){
                    data[data_counter] = (byte)arr[k];
                    data_counter++;
                }
            }
        }

        // Add fire alarm state to the data array
        data[data_counter] = (byte)0;
        for(int i = 0; i < 2; i++){
            data[data_counter] |= (byte)((int)fireAlarmState[i] << i);
        }
        // Add water alarm state to the data array
        for (int i = 0; i < 6; i++){
            data[data_counter] |= (byte)(((int)waterAlarmState[i]) << (2 + i));
        }
        data_counter += 1;
        // Add land power alarm state to the data array
        data[data_counter] |= (byte)((int)powerAlarmState);
        data_counter += 1;

        // Sending the data array  
        Serial.println("Sending this array of data to the server: ");
        for(int i = 0; i < data_counter; i++){
            Serial.print((byte)data[i]);
            Serial.print(" ");
        }
        Serial.println(" ");
        if(GPRS_send(data, data_counter)){                      //Sending the data to the server.
            Serial.println("Data was successfully sent!");
        }
        else{
            Serial.println("ERROR: Failed to send data");
            numErrSend++;
            if(numErrSend >= 3){
                // Connection is down, restarting the Arduino
                restartArduino();
            }
        }
        data_counter = 15;           //Resetting the number of entries in data.
        delay(1000);
        if(NTP_sync()){
            //Time sync
            ts_synced = get_unix_ts();                      //Synchronizing time
            setTime(ts_synced);                             //Sets local Arduino time to the server time
            Serial.print("Time sync: ");
            Serial.println(ts_synced);
            Serial.println("Synchronized time successfully!");
        }
        else{
            Serial.println("Did not sync time successfully!");
        }
        timeToSendData = false;
    }
}

void sendAlarmSMS(char alarmType, int alarmNumber){
    if(alarmType == 'F'){
        switch(alarmNumber){
            case 0:
                Serial.println("Fire in the engine room - Sending SMS");
                // sendSMS(phoneNumber, "Det brenner i motorrommet på båten!");
                break;
            case 1:
                Serial.println("Fire in the kitchen - Sending SMS");
                // sendSMS(phoneNumber, "Det brenner i lugaren på båten!");
                break;
            default:
                Serial.println("Unknown fire alarm number - Did not send any SMS");
        }
    }
    else if(alarmType == 'W'){
        switch(alarmNumber){
            case 0:
                if(waterAlarmState[1] == true || waterAlarmState[2] == true){
                    Serial.println("Water in the engine room - Sending SMS");
                    //sendSMS(phoneNumber, "Det er vann i motorrommet på båten!");
                }
                break;
            case 1:
                if(waterAlarmState[0] == true || waterAlarmState[2] == true){
                    Serial.println("Water in the engine room - Sending SMS");
                    //sendSMS(phoneNumber, "Det er vann i motorrommet på båten!");
                }
                break;
            case 2:
                if(waterAlarmState[0] == true || waterAlarmState[1] == true){
                    Serial.println("Water in the engine room - Sending SMS");
                    //sendSMS(phoneNumber, "Det er vann i motorrommet på båten!");
                }
            case 3:
                if(waterAlarmState[4] == true || waterAlarmState[5] == true){
                    Serial.println("Water in the lounge - Sending SMS");
                    //sendSMS(phoneNumber, "Det er vann i salongen på båten!");
                }
                break;
            case 4:
                if(waterAlarmState[3] == true || waterAlarmState[5] == true){
                    Serial.println("Water in the lounge - Sending SMS");
                    //sendSMS(phoneNumber, "Det er vann i salongen på båten!");
                }
                break;
            case 5:
                if(waterAlarmState[3] == true || waterAlarmState[4] == true){
                    Serial.println("Water in the lounge - Sending SMS");
                    //sendSMS(phoneNumber, "Det er vann i salongen på båten!");
                }
                break;
            default:
                Serial.println("Unknown waterAlarm alarm number - Did not send any SMS");
        }
    }
    else if(alarmType == 'P'){
        switch(alarmNumber){
            case 0:
                Serial.println("No land power! - Sending SMS");
                //sendSMS(phoneNumber, "Landstrømmen på båten er koblet ut!");
                break;
            default:
                Serial.println("Unknown landPowerAlarm number - Did not send any SMS");
        }
    }
    else{
        Serial.println("Unknown alarm type - Did not send any SMS");
    }
    
}

void timeCounter(int *timeStart, int timeStop, int *tmpA, int *tmpB, bool *state){
    if(*timeStart >= timeStop){
        *state = !(*state);
    }
    *tmpB = minute();
    if(*tmpA != *tmpB){
        (*timeStart)++;
        *tmpA = minute();
    }
}






