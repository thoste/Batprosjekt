#include <Arduino.h>
#include "Time.h"
#include "HardwareLink3.h"
#include "DallasTemperature.h"

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
 
int8_t answer;
int onModulePin = 2;
int button = 12;
char aux_str[30];

char phone_number[]="******";     // ********* is the number to call
char pin[] = "****";

void setup(){
    Serial.begin(9600);
    pinMode(onModulePin, OUTPUT);
    pinMode(button, INPUT);
    Serial3.begin(115200);      
    
    Serial.println("Starting...");
    power_on();
    
    delay(3000);
    
    // sets the PIN code
    //sprintf(aux_str, "AT+CPIN=%s", pin);
    sendATcommand(aux_str, "OK", 2000);

    
    delay(3000);
    
    Serial.println("Connecting to the network...");

    while( (sendATcommand("AT+CREG?", "+CREG: 0,1", 500) || 
            sendATcommand("AT+CREG?", "+CREG: 0,5", 500)) == 0 );

    sprintf(aux_str, "ATD%s;", phone_number);
    sendATcommand(aux_str, "OK", 10000);
    
    // press the button for hang the call 
    while(digitalRead(button)==1);        

    Serial.println("ATH");            // disconnects the existing call
    
    ip_adr = get_IP();                              //Holding the IP address given to us by the telecom operator.
    Serial.print("IP address: ");
    Serial.println(ip_adr);
    

}

void loop(){

}

void power_on(){

    uint8_t answer=0;
    
    // checks if the module is started
    answer = sendATcommand("AT", "OK", 2000);
    if (answer == 0)
    {
        // power on pulse
        digitalWrite(onModulePin,HIGH);
        delay(3000);
        digitalWrite(onModulePin,LOW);
    
        // waits for an answer from the module
        while(answer == 0){     // Send AT every two seconds and wait for the answer
            answer = sendATcommand("AT", "OK", 2000);    
        }
    }
    
}

int8_t sendATcommand(char* ATcommand, char* expected_answer, unsigned int timeout){

    uint8_t x=0,  answer=0;
    char response[100];
    unsigned long previous;

    memset(response, '\0', 100);    // Initialize the string
    
    delay(100);
    
    while( Serial3.available() > 0) Serial3.read();    // Clean the input buffer
    
    Serial3.println(ATcommand);    // Send the AT command 


    x = 0;
    previous = millis();

    // this loop waits for the answer
    do{
        if(Serial3.available() != 0){    
            // if there are data in the UART input buffer, reads it and checks for the asnwer
            response[x] = Serial3.read();
            x++;
            // check if the desired answer  is in the response of the module
            if (strstr(response, expected_answer) != NULL)    
            {
                answer = 1;
            }
        }
         // Waits for the asnwer with time out
    }while((answer == 0) && ((millis() - previous) < timeout));   

    return answer;
}
                