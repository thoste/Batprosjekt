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
unsigned long timetime = 0;

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

//TEMP
bool tempSampleState = false;
int iTemp = 0;
double tempStore[3][3] = {0};
int minuteTempSampleStart = 0;
int minuteTempSampleStopp = 0;
int tmpTempA = 0;
int tmpTempB = 0;


//FIRE
const int fireAlarmPin[2] = {18,19}; //Sets the pin input on the arduino for the fire alarm pins
bool fireAlarmState[2] = {0};          //Holds the fire alarm boolean states - default is false
int minuteFireAlarmStart[2] = {0};
int minuteFireAlarmStopp[2] = {0};
int tmpFireA[2] = {0};
int tmpFireB[2] = {0};

//WATER
const int waterAlarmPin[6] = {1,2,3,4,5,6}; //Sets the pin input on the arduino for the water alarm pins
bool waterAlarmState[6] = {0};          //Holds the water alarm boolean states - default is false
int counterWaterAlarm[6] = {0};
int minuteWaterAlarmStart[6] = {0};
int minuteWaterAlarmStopp[6] = {0};
int tmpWaterA[6] = {0};
int tmpWaterB[6] = {0};


void setup(){
    // Serial used for debugging
    Serial.begin(9600);

    setTime(1458031892); // For now...

    // // Modem startup
    // Serial3.begin(4800);                            //We use Serial3 for communication with the modem.
    // Serial.println(F("Modem booting"));
    // modemStart();    // Boot modem and enter PIN    
    // Serial.println(F("Modem boot completed"));      
    // Serial.println(F("Entering modem setup"));
    // if(GPRS_setup()){                               //Configures the modem so that it is able to send data and SMS.
    //     Serial.println(F("Modem setup completed"));   
    // }
    // else{
    //     Serial.println(F("Modem setup failed"));
    //     Serial.println(F("Restarting the Arduino"));
    //     // Restart Arduino
    // }
    // ip_adr = get_IP();                              //Holding the IP address given to us by the telecom operator.
    // Serial.print("IP address: ");
    // Serial.println(ip_adr);
    // IMEI_nr = get_IMEI_nr();                        //Holding the IMEI number of the modem.
    // for(int i = 0; i < 15; i++){                    //Adding the IMEI number to data.
    //     data[i] = IMEI_nr[i];
    // }
    // Serial.print("IMEI number: ");
    // for(int i = 0; i < 15; i++){
    //     Serial.print((char)data[i]);
    // }
    // Serial.println(" ");
    // //Time sync
    // ts_synced = get_unix_ts();                      //Synchronizing time
    // setTime(ts_synced);                             //Sets local Arduino time to the server time
    // Serial.print("Time sync: ");
    // Serial.println(ts_synced);
    // Serial.print("Signal strenght: ");
    // Serial.println(getSignalStrength());
    // if(GPRS_ping(ping_adr)){                        //Pinging the Internet.
    //     Serial.print("Pinged "); 
    //     Serial.print(ping_adr);
    //     Serial.println(" successfully");
    // }
    // else{
    //     Serial.print("ERROR: Could not ping ");
    //     Serial.println(ping_adr);
    // }

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

    delay(1000);

    // sendSMS(phoneNumber, "Arduinbo bootup complete!");

    // After setup its time to sample the temperatures
    tempSampleState = true;

    Serial.println("Setup completed!");
}

void loop(){

	temperature();

     fireAlarm(0,fireAlarmPin[0]);
     fireAlarm(1,fireAlarmPin[1]);

    // waterAlarm(0,waterAlarmPin[0]);
    // waterAlarm(1,waterAlarmPin[1]);
    // waterAlarm(2,waterAlarmPin[2]);
    // waterAlarm(3,waterAlarmPin[3]);
    // waterAlarm(4,waterAlarmPin[4]);
    // waterAlarm(5,waterAlarmPin[5]);

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
            sendAlarmSMS('W', alarmNumber);
			counterWaterAlarm[alarmNumber] = 0;
			minuteWaterAlarmStart[alarmNumber] = minute();
            tmpWaterA[alarmNumber] = minute();
			minuteWaterAlarmStopp[alarmNumber] = minuteWaterAlarmStart[alarmNumber] + 1;
            waterAlarmState[alarmNumber] = true;
			break;
		}
	}
	if(waterAlarmState[alarmNumber] == true){
		timeCounter(&minuteWaterAlarmStart[alarmNumber], minuteWaterAlarmStopp[alarmNumber], &tmpWaterA[alarmNumber], &tmpWaterB[alarmNumber], &waterAlarmState[alarmNumber]);
	}	
}

void temperature(){
    while(tempSampleState == true){
        sensors.requestTemperatures();
        tempStore[0][iTemp] = sensors.getTempCByIndex(0);
        tempStore[1][iTemp] = sensors.getTempCByIndex(1);
        tempStore[2][iTemp] = sensors.getTempCByIndex(2);
        Serial.println("Temp store round");
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
        for(int i = 0; i < 4; i++){
            data[data_counter + i] = (byte)((int)tempStore[0][0] >> 8*(3 - i));
            data[data_counter + i + 4] = (byte)((int)tempStore[0][1] >> 8*(3 - i));
            data[data_counter + i + 8] = (byte)((int)tempStore[0][2] >> 8*(3 - i));
            data[data_counter + i + 12] = (byte)((int)tempStore[1][0] >> 8*(3 - i));
            data[data_counter + i + 16] = (byte)((int)tempStore[1][1] >> 8*(3 - i));
            data[data_counter + i + 20] = (byte)((int)tempStore[1][2] >> 8*(3 - i));
            data[data_counter + i + 24] = (byte)((int)tempStore[2][0] >> 8*(3 - i));
            data[data_counter + i + 28] = (byte)((int)tempStore[2][1] >> 8*(3 - i));
            data[data_counter + i + 32] = (byte)((int)tempStore[2][2] >> 8*(3 - i));
        }
        data_counter +=  36;

        // Add fire and water alarm state to the data array
        data[data_counter] = (byte)(((int)fireAlarmState[0]) | (((int)fireAlarmState[1]) << 1));

        for (int i = 0; i < 6; i++){
            data[data_counter] = (byte)(((int)waterAlarmState[0]) << (2 + i));
        }
        data_counter += 1;

        // Sending the data array  
        Serial.println("Sending this array of data to the server: ");
        for(int i = 0; i < data_counter; i++){
            Serial.print((byte)data[i]);
            Serial.print(" ");
        }
        Serial.println(" ");
        // if(GPRS_send(data, data_counter)){                      //Sending the data to the server.
        //     Serial.println("Data was successfully sent!");
        // }
        // else{
        //     Serial.println("ERROR: Failed to send data");
        // }
        data_counter = 15;           //Resetting the number of entries in data.
        // if(NTP_sync()){
        //     Serial.println("Synchronized time successfully!");
        // }
        // else{
        //     Serial.println("Did not sync time successfully!");
        // }
        timeToSendData = false;
    }
}


void sendAlarmSMS(char alarmType, int alarmNumber){
    switch(alarmType){
        case 'F': 
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
            break;
        case 'W':
            switch(alarmNumber){
                case 0:
                    Serial.println("Water in the engine room - Sending SMS");
                    //sendSMS(phoneNumber, "Det er vann i motorrommet på båten!");
                    break;
                case 1:
                    Serial.println("Water in the lounge - Sending SMS");
                    //sendSMS(phoneNumber, "Det er vann i salongen på båten!");
                    break;
                default:
                    Serial.println("Unknown waterAlarm alarm number - Did not send any SMS");
            }
            break;
        default:
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






