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
unsigned long millis_at_ts_sync = 0;          //millis() at the time of unix time synchronization.
unsigned long ts_now = 0;                     //Unix time stamp right now. Calculated with ts_synced and millis().
unsigned long millis_now = 0;                 //millis() right now.
byte ts_bytes[4] = {};                        //Array holding a time stamp (a long variable) that is cast to an array of 4 bytes.
bool ts_is_added_this_loop = false;           //Keeps track of if time stamp is added to the current row or not.
bool first_loop = true;                       //Is set to false at the end of the very first loop.

//DATA
bool timeToSendData = false;
byte data[2048] = {};                         //The array of bytes we send to the server.
long data_counter = 15;                       //Number of bytes in the data array. It is initialized to 15 because we already added the IMEI number digit by digit.                                         
long data_counter_start;                      //Data counter at start of loop.
long data_send_interval = 30000;              //The time between transfers of data to the server.
unsigned long millis_at_last_send = 0;        //millis() at last transfer.
char* ip_adr;                                 //C-string to hold the address we are assigned from the telecom operator.
char ping_adr[32] = "www.google.com";         //The address we wish to ping.

//TEMP
bool tempSampleState[3] = {1};
int iTemp[3] = {0};
int jTemp[3] = {0};
double temps[3][10] = {0};
double tempStore[3][3] = {0};
int minuteTempSampleStart[3] = {0};
int minuteTempSampleStopp[3] = {0};
int tmpTempA[3] = {0};
int tmpTempB[3] = {0};


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
    // millis_at_ts_sync = millis();                   //Saving millis() at time of synchronization
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
    // //Resetting time intervals
    // millis_now = millis();
    // millis_at_last_send = millis_now;

    // Temp sensors begin
    sensors.begin();

    // Fire and water sensor pins
    pinMode(fireAlarmPin[0], INPUT);
    pinMode(fireAlarmPin[1], INPUT);

    for (int i = 0; i < 6; i++){
        pinMode(waterAlarmPin[i], INPUT);
    }

    //sendSMS(phoneNumber, "Arduinbo bootup complete!");
}

void loop(){
    // Serial.print("Time: ");
    // Serial.print(hour());
    // Serial.print(":");
    // Serial.print(minute());
    // Serial.print(":");
    // Serial.println(second());


	temperature(0);
    temperature(1);
    temperature(2);

    // fireAlarm(0,fireAlarmPin[0]);
    // fireAlarm(1,fireAlarmPin[1]);

    // for(int i = 0; i < 6; i++){
    //     waterAlarm(i,waterAlarmPin[i]);
    // }

    sendDataToServer();

	//delay(100);
}

void fireAlarm(int alarmNumber, int alarmPin){
	while(digitalRead(alarmPin) == HIGH && fireAlarmState[alarmNumber] != true){
		Serial.print("Fire alarm for number: ");
        Serial.println(alarmNumber);
		sendAlarmSMS('F', alarmNumber);
		minuteFireAlarmStart[alarmNumber] = minute();
        tmpFireA[alarmNumber] = minute();
		minuteFireAlarmStopp[alarmNumber] = minuteFireAlarmStart[alarmNumber] + 1;
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

void temperature(int tempSensor){
    while(tempSampleState[tempSensor] == true){
        double tmp = 0;
        sensors.requestTemperatures();
        temps[tempSensor][(iTemp[tempSensor])] = sensors.getTempCByIndex(tempSensor);
        iTemp[tempSensor]++;
        if(iTemp[tempSensor] == 10){
            for(int i = 0; i < iTemp[tempSensor]; i++){
                tmp += temps[tempSensor][i];
            }
            tempStore[tempSensor][(jTemp[tempSensor])] = tmp/iTemp[tempSensor];
            Serial.println("Temp store round");
            if(jTemp[tempSensor] == 2){
                Serial.print("Temp store done 3 times for temp sensor: ");
                Serial.println(tempSensor);
                if(tempSensor == 2){
                    Serial.println("Temp store done for all, time to send data to server");
                    timeToSendData = true;
                }
                jTemp[tempSensor] = 0;
            }
            jTemp[tempSensor]++;
            minuteTempSampleStart[tempSensor] = minute();
            tmpTempA[tempSensor] = minute();
            minuteTempSampleStopp[tempSensor] = minuteTempSampleStart[tempSensor] + 1;
            tempSampleState[tempSensor] = false;
            tmp = 0;
            iTemp[tempSensor] = 0;
        }
    }
    if(tempSampleState[tempSensor] != true){
            timeCounter(&minuteTempSampleStart[tempSensor], minuteTempSampleStopp[tempSensor], &tmpTempA[tempSensor], &tmpTempB[tempSensor], &tempSampleState[tempSensor]);
        }
}


void sendDataToServer(){
    if(timeToSendData == true){
        Serial.println("Sending data to server");
        // Send data
        timeToSendData = false;
    }
}


void sendAlarmSMS(char alarmType, int alarmNumber){
    switch(alarmType){
        case 'F': 
            switch(alarmNumber){
                case 0:
                    Serial.println("Fire in the engine room - Sending SMS");
                    //sendSMS(phoneNumber, "Det brenner i motorrommet på båten!");
                    break;
                case 1:
                    Serial.println("Fire in the kitchen - Sending SMS");
                    //sendSMS(phoneNumber, "Det brenner i lugaren på båten!");
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
        Serial.println("Alarm has been reset");
    }
    *tmpB = minute();
    if(*tmpA != *tmpB){
        (*timeStart)++;
        *tmpA = minute();
    }
}






