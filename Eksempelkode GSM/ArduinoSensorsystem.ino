/**********/
/*INCLUDES*/
/**********/
#include <Arduino.h>
#include <Time.h>
#include <HardwareLink3.h>





/**************/
/*ABBREVIATION*/
/**************/
/*

sp = signal processing
ts = time stamp (unix time format)
sync = time synchronization with the modem (unix time format)
tmp = temperature
IMEI = International Mobile Equipment Identity (identity of the modem) (15 digits)

*/




/*************/
/*INITIATIONS*/
/*************/
//MODEM
long pincode = 0000;                          //The PIN code of the SIM card inserted in the modem.
byte* IMEI_nr = {};                           //Array that holds the IMEI number of the modem.

//TIMESTAMP
unsigned long ts_synced = 0;                  //Synchronized unix time stamp.
unsigned long millis_at_ts_sync = 0;          //millis() at the time of unix time synchronization.

unsigned long ts_now = 0;                     //Unix time stamp right now. Calculated with ts_synced and millis().
unsigned long millis_now = 0;                 //millis() right now.

byte ts_bytes[4] = {};                        //Array holding a time stamp (a long variable) that is cast to an array of 4 bytes.

bool ts_is_added_this_loop = false;           //Keeps track of if time stamp is added to the current row or not.

bool first_loop = true;                       //Is set to false at the end of the very first loop.

//TEMPERATURE SENSOR
int tmp_code = 2;                             //The chosen sensor code number for the temperature sensor is 2.
int tmp_pin = 0;                              //The chosen pin for the temperatur sensor is A0.
long tmp_sample_interval = 7500;              //Time between temperature samples.
long tmp_sp_interval = 15000;                 //Time between signal processing of samples.
int tmp_samples[255] = {};                    //Array to hold samples.
int tmp_counter = 0;                          //Number of samples in the array.
unsigned long millis_at_last_tmp_sample = 0;  //millis() at last sample.
unsigned long millis_at_last_tmp_sp = 0;      //millis() at last signal processing of the samples.

//PUMP SENSOR
int pump_code = 4;
int pump_pin = 53;
long pump_sample_interval = 30000;
long pump_sp_interval = 30000;
int pump_samples[255] = {};
int pump_counter = 0;
unsigned long millis_at_last_pump_sample = 0;
unsigned long millis_at_last_pump_sp = 0;

bool pump_alert = false;                      //If true we send a warning to the owner that the bilge pump has stopped.

//DATA
byte data[2048] = {};                         //The array of bytes we send to the server.
long data_counter = 15;                       //Number of bytes in the data array. 
                                              //It is initialized to 15 because we already added the IMEI number digit by digit.
                                              
long data_counter_start;                      //Data counter at start of loop.
long data_send_interval = 30000;              //The time between transfers of data to the server.
unsigned long millis_at_last_send = 0;        //millis() at last transfer.

char* ip_adr;                                 //C-string to hold the address we are assigned from the telecom operator.
char ping_adr[32] = "www.google.com";         //The address we wish to ping.









/*******/
/*SETUP*/
/*******/
void setup() {
  
  //Serial monitor
  Serial.begin(9600);                             //We wish to use the Serial Monitor for debugging.

  //Modem
  Serial3.begin(4800);                            //We use Serial3 for communication with the modem.

  Serial.println(F("Modem booting"));
  modemStart(pincode);                            //Boots the modem and enters the pin code.
  Serial.println(F("Modem boot completed"));
  Serial.println(F("Entering modem setup"));
  if(GPRS_setup()){                               //Configures the modem so that it is able to send data and SMS.
    Serial.println(F("Modem setup completed"));   
  }else{
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
  }else{
    Serial.print("ERROR: Could not ping ");
    Serial.println(ping_adr);
  }


  //Resetting time intervals
  millis_now = millis();
  
  millis_at_last_tmp_sample = millis_now;
  millis_at_last_tmp_sp = millis_now;
    
  millis_at_last_pump_sample = millis_now;
  millis_at_last_pump_sp = millis_now;
    
  millis_at_last_send = millis_now;
  
}//end of setup()







/******/
/*LOOP*/
/******/
void loop() {

  /*** Timestamp & counters ***/
  
  millis_now = millis();                        //We update millis_now every loop, 
                                                //and use this variable to calculate intervals, 
                                                //so that the time does not increment mid-loop.
  
  ts_now = ts_synced + ((unsigned long)(millis_now - millis_at_ts_sync) / 1000);    //We update ts_now every loop in case it will be added to a row.
  ts_bytes[0] = (byte)(ts_now >> 24) & 255;     
  ts_bytes[1] = (byte)(ts_now >> 16) & 255;
  ts_bytes[2] = (byte)(ts_now >> 8) & 255;
  ts_bytes[3] = (byte)(ts_now & 255);           //We cast the long variable to an array of bytes.

  data_counter_start = data_counter;            //We save the number of bytes in data at the start of every loop.

  ts_is_added_this_loop = false;                //Time stamp is not yet added this loop.


  /*** Pins ***/
  
  pinMode(pump_pin, INPUT);

  /*** Sampling ***/

  //Sampling temperature
  if((unsigned long)(millis_now - millis_at_last_tmp_sample) >= tmp_sample_interval){   //If enough time has passed since last sample.
    millis_at_last_tmp_sample += tmp_sample_interval;                                   //Update the time at last sample
    tmp_samples[tmp_counter] = tmp_take_sample(tmp_pin);                                //Add a sample
    tmp_counter++;                                                                      //Increment the counter
    
  }

  //Sampling pump
  if((unsigned long)(millis_now - millis_at_last_pump_sample) >= pump_sample_interval){
    millis_at_last_pump_sample += pump_sample_interval;
    pump_samples[pump_counter] = digitalRead(pump_pin);
    pump_counter++;
  }
  
  /*** Signal processing ***/

  //Processing temperature samples
  if((unsigned long)(millis_now - millis_at_last_tmp_sp) >= tmp_sp_interval){   //If enough time has passed since last signal processing.
    millis_at_last_tmp_sp += tmp_sp_interval;                                   //Update the time at last signal processing

    if(!ts_is_added_this_loop){                                                 //We add the time stamp if it is not added
      for(int i = 0; i < 4; i++){
        data[data_counter] = ts_bytes[i];
        data_counter++;
      }
      ts_is_added_this_loop = true;
    }

    //Adding the result of signal processing to data
    data[data_counter] = tmp_code;
    data_counter++;
    data[data_counter] = tmp_sp(tmp_samples, tmp_counter);
    data_counter++;

    //Resetting number of samples to zero
    tmp_counter = 0;


    //ADDING WIND VALUE OUT OF THE BLUE (VIRTUAL SENSOR)
    data[data_counter] = 8;
    data_counter++;
    data[data_counter] = random(1, 4);
    data_counter++;
    
  }//end of processing temperature samples
  

  //Processing pump samples
  if((unsigned long)(millis_now - millis_at_last_pump_sp) >= pump_sp_interval){
    millis_at_last_pump_sp += pump_sp_interval;

    if(!ts_is_added_this_loop){
      for(int i = 0; i < 4; i++){
        data[data_counter] = ts_bytes[i];
        data_counter++;
      }
      ts_is_added_this_loop = true;
    }

    //Adding result to data
    data[data_counter] = pump_code;
    data_counter++;
    data[data_counter] = pump_samples[pump_counter-1];
    data_counter++;

    //We send a warning on sms if the pump stops and we have not sent a warning already.
    if(pump_samples[pump_counter-1] == 0 && pump_alert == 0){
      Serial.println("The bilgepump is currently inactive, and we send this (unimportant) information to the owner via SMS");
      sendSMS("98480712", "Lensepumpen er ikke aktiv for øyeblikket. Hilsen Båtvakten");
      pump_alert = true;
    }

    //ADDING WATERLEVEL VALUE OUT OF THE BLUE (VIRTUAL SENSOR)
    data[data_counter] = 3;
    data_counter++;
    if(pump_samples[pump_counter-1]){
      data[data_counter] = random(0, 2);
    }else{
      data[data_counter] = random(2, 5);
    }
    data_counter++;

    //Resetting number of samples to zero
    pump_counter = 0;
    
  }//end of processing pump samples
  

  //If we added values to data we add the terminating value 99 which terminates the row.
  if(data_counter > data_counter_start){
    data[data_counter] = 99;
    data_counter++;
  }

  /*** Sending ***/
  
  if((unsigned long)(millis_now - millis_at_last_send) >= data_send_interval){  //If enough time has passed since last data transfer.
    millis_at_last_send += data_send_interval;                                  //Updating time at last transfer.
    
    Serial.print("Sending this array of data to the server: ");
    for(int i = 0; i < data_counter; i++){
      Serial.print((int)data[i]);
      Serial.print(" ");
    }
    Serial.println(" ");
    
    if(GPRS_send(data, data_counter)){                      //Sending the data to the server.
      Serial.println("Data was successfully sent!");
    }else{
      Serial.println("ERROR: Failed to send data");
    }

    data_counter = 15;           //Resetting the number of entries in data.

    if(NTP_sync()){
      Serial.println("Synchronized time successfully!");
    }else{
      Serial.println("Did not sync time successfully!");
    }
  }

  
}//end of loop()







/***********/
/*FUNCTIONS*/
/***********/

/*** Temperature ***/

//Returns a temperature sample
int tmp_take_sample(int sensor_pin){
  int sensor_val = analogRead(sensor_pin);
  float voltage = (sensor_val/1024.0) * 5.0;
  return (voltage - .5) * 100;
}

//returns the result of signal processing of the temperature samples
int tmp_sp(int* samples, int len){
    long sum = 0;
    double d = 0;
    int res = 0;
    
    for(int i = 0; i < len; i++){
      sum += samples[i];
    }
    
    d = sum / (double)len;
    res = (d >= 0.0 ? (int)(d + 0.5) : (int)(d - 0.5));

    return res;
  
}

