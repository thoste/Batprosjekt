// Phone number of the boat owner
char* phoneNumber = "95174794";

// Time variables
unsigned long unix_time_ts_sync = 0;
unsigned long millis_at_ts_sync = 0;

// Temperature sensors
double temp1;
double temp2;
double tempM;

// Water alarm
bool waterAlarmState = false;
int counterWaterAlarm = 0;
int minuteWaterAlarmStart = 0;
int minuteWaterAlarmStopp = 0;
int minutesSinceWaterAlarm = 0;
int tmpA = 0;
int tmpB = 0;

// Fire alarm
bool fireAlarmState = false;
bool sendtFireAlarmSMS = false;
int minuteFireAlarmSMSsendt = 0;






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