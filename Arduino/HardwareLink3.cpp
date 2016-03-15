#include "HardwareLink3.h"


//Checks if input str ends with 'OK(submit)(newline)'. ASCII value 13 = submit. ASCII value 10 = newline.
//Is used to keep the code from writing the next command until the modem has confirmed the previous command.
bool cmdOK(char* str){
	int l = cstringLength(str);
	if((str[l-4] == 'O') && (str[l-3] == 'K') && (str[l-2] == 13) && (str[l-1] == 10)){
		return true;
	}
	return false;
}

//Checks if str ends with '>'.
bool rdy2write(char* str){
	int l = cstringLength(str);
	return ((str[l-1]) == '>');
}

//Checks if str ends with 'PIN'.
bool rdy4pin(char* str){
	int l = cstringLength(str);
	if((str[l-3] == 'P') && (str[l-2] == 'I') && (str[l-1] == 'N')){
		return true;
	}
	return false;
}

//Checks if str ends with 'DST'.
bool bootFinished(char* str){
	int l = cstringLength(str);
	if(l >= 3){
		if((str[l-3] == 'D') && (str[l-2] == 'S') && (str[l-1] == 'T')){
			return true;
		}
	}	
	return false;
}

//Checks if str ends with 'FAIL'.
bool connFailed(char* str){
	int l = cstringLength(str);
	if(l >= 4){
		if((str[l-4] == 'F') && (str[l-3] == 'A') && (str[l-2] == 'I') && (str[l-1] == 'L')){
			return true;
		}
	}
	return false;
}

//Checks if str ends with 'ERROR'.
bool cmdError(char* str){
	int l = cstringLength(str);
	if(l >= 5){
		if((str[l-5] == 'E') && (str[l-4] == 'R') && (str[l-3] == 'R') && (str[l-2] == 'O') && (str[l-1] == 'R')){
			return true;
		}
	}
	return false;
}







//Writes pin code to modem, and waits until modem is finished booting.
void modemStart(){
	char str[128] = ""; 	//String to gather answer from modem.
	
	pinMode(8, OUTPUT);		//Pin 8 is PWRKEY pin on the GSM-shield.
    pinMode(9, OUTPUT);		//Pin 9 is RESTART pin on the GSM-shield.
    digitalWrite(9, LOW);	//Setting both to low.
    digitalWrite(8, LOW);	
    delay(5000);
    digitalWrite(8, HIGH); // Power GSM shield
    delay(1000);
    digitalWrite(8, LOW);
    Serial.println("Power GSM shield");
    delay(5000);
    digitalWrite(9, HIGH);	// Reset GSM shiled
    delay(1000);
    digitalWrite(9, LOW);	
    Serial.println("Reset GSM shield");
    delay(5000);
	
	long loopcounter = 0;
	
	while(!rdy4pin(str)){				//Waits until the modem asks for pin code.
		while(Serial3.available()){
			cstringAppend(str, (char)Serial3.read());
		}
		if(loopcounter > 5000){			//If enough time has passed, we presume the modem is off.
			digitalWrite(8, HIGH);		//We boot the modem.
			delay(1000);
			digitalWrite(8, LOW);
			loopcounter = 0;
			
			str[0] = '\0';
		}
		
		delay(1);
		loopcounter++;
	}
	str[0] = '\0';
	loopcounter = 0;
	flushReg();
	submit(0);
	while(!bootFinished(str)){		//Waits until boot is finished.
		if(Serial3.available()){
			cstringAppend(str, (char)Serial3.read());	
		}
		if(loopcounter > 30000){			//If enough time has passed, the modem is stuck, we restart the booting process
			Serial.println("Did not work.. Restarting the booting process");
			digitalWrite(8, HIGH); // Power GSM shield
    		delay(1000);
    		digitalWrite(8, LOW);
    		delay(5000);
    		digitalWrite(9, HIGH);	// Reset GSM shiled
    		delay(1000);
    		digitalWrite(9, LOW);
    		loopcounter = 0;
    		str[0] = '\0';
    		flushReg();
			modemStart();
		}
		delay(1);
		loopcounter++;
	}
	str[0] = '\0';
	return;
}

bool GPRS_setup() {
	const char clts[] PROGMEM = "AT+CLTS=1"; //Enable time update
	const char cgatt[] PROGMEM = "AT+CGATT=1"; //Attach to GPRS service
	const char cipmux[] PROGMEM = "AT+CIPMUX=0"; //Configure single-IP connection
	const char cstt[] PROGMEM = "AT+CSTT=\"telenor\",\"dj\",\"dj\""; //Start task, set APN, username and password
	const char ciicr[] PROGMEM = "AT+CIICR"; //Bring up wireless connection with GPRS.
	const char sapbr1[] PROGMEM = "AT+SAPBR=3,1,\"Contype\",\"GPRS\""; //Activate bearer profile
	const char sapbr2[] PROGMEM = "AT+SAPBR=3,1,\"APN\",\"telenor\""; //Activate bearer profile
	const char sapbr3[] PROGMEM = "AT+SAPBR=3,1,\"USER\",\"dj\""; //Activate bearer profile
	const char sapbr4[] PROGMEM = "AT+SAPBR=3,1,\"PWD\",\"dj\""; //Activate bearer profile
	const char sapbr5[] PROGMEM = "AT+SAPBR=1,1"; //Activate bearer profile
	const char cntp1[] PROGMEM = "AT+CNTP=\"no.pool.ntp.org\",1,1,0";  //Connect to NTP server
	const char cntp2[] PROGMEM = "AT+CNTP";  //Get network time
	const char* commands[] = {clts, cgatt, cipmux, cstt, ciicr, sapbr1, sapbr2, sapbr3, sapbr4, sapbr5, cntp1, cntp2};
	for(int i = 0; i < 12; i++){
		Serial.println(i);
		Serial.println((commands[i]));
		print3(commands[i]); //Writes command to GSM-shield
		if(!waitForOk(commands[i])){ //Waiting for OK from GSM-shield, retries 1 time if GSM-shield returns ERROR
			return false; //Exit GPRS_setup() if command fails
		}
	}
	flushReg();
	return true;
}

 
void print3(const char* str){
	flushReg();
	Serial3.print(str);
	submit(0);
}


bool waitForOk(const char* command){
	char str[128] = "";
	bool fail = false;
	while (!cmdOK(str)) {
		if (Serial3.available()) {
			cstringAppend(str, (char)Serial3.read());
		}
		if (cmdError(str)) {
			if(fail){
				fail = false;
				return false;
			}
			else{
				fail = true;
			}
		}
		if(fail){
			print3(command);
		}
	}
	str[0] = '\0';
	return true;
}

//Sends SMS and returns true if successful and false if not.
bool sendSMS(char* num,char* msg){
        if(cstringLength(msg) > 160){			//Maximum size for an SMS is 160 letters.
                return false;
        }
		flushReg();
        Serial3.print(F("AT+CMGF=1"));			//Configures SMS format to text mode
        submit(500);
		flushReg();
        Serial3.print(F("AT+CSCS=\"HEX\""));	//Configures SMS character set to HEX
        submit(500);
		flushReg();
        Serial3.print(F("AT+CMGS=\"2b3437"));			//Sending the SMS. Adds '+47' in HEX at the start of the phone number
        for(int i = 0; i < cstringLength(num); i++){	//Adds phone number in HEX.
                Serial3.print((int)num[i],HEX);
        }
        Serial3.print(F("\""));
        Serial3.write(13);
        delay(500);
        for(int i = 0; i < cstringLength(msg); i++){	//Writes message in HEX.

				
                if((int)msg[i] == -61){			//If special letter.
                        switch((int)msg[i+1]){
                                case (-122):	//Æ.
                                        Serial3.print(28,HEX);
                                        break;
                                case (-104): 	//Ø.
                                        Serial3.print(0);
                                        Serial3.print(11,HEX);
                                        break;
                                case (-123):	//Å.
                                        Serial3.print(0);
                                        Serial3.print(14,HEX);
                                        break;                         
                                case (-90):		//æ.
                                        Serial3.print(29,HEX);
                                        break;
                                case (-72):		//ø.
                                        Serial3.print(0);
                                        Serial3.print(12,HEX);
                                        break;
                                case (-91):		//å.
                                        Serial3.print(0);
                                        Serial3.print(15,HEX);
                                        break; 
                        }
                        i++;
                }else {
                        Serial3.print((int)msg[i],HEX);	
                }
                       
        }
        Serial3.write(26);
        submit(0);
        char ret[160+64] = "";
        char c;
		int count = 0;
		
		while(!cmdOK(ret)){ 			//Waits for answer
			if(Serial3.available()){
				cstringAppend(ret, (char)Serial3.read());
			}
			delay(100);
			count++;
			
			if(count > 250){
				return false;
			}
		}
		
        return cmdOK(ret);
}

//Returns signal strength.
int getSignalStrength(){
	flushReg();
	Serial3.print(F("AT+CSQ"));				//Asks for signal strength
	submit(500); 							
	char ret[128] = "";						//String to gather what SIM900 returns
	char signalStrength[2] = "";		
	char c;
	while(Serial3.available()){
		cstringAppend(ret, (char)Serial3.read() );
	}
	if(cmdOK(ret)){					//If the command went through
		int l = cstringLength(ret);
		cstringAppend(signalStrength, ret[l-12]);		//The signal strength will appear at the 12th and 11th last position in the answer from the modem.
		cstringAppend(signalStrength, ret[l-11]);
		return cstring2int(signalStrength);
	}else{
		return -1;
	}
}


//Sends array of bytes to the server.
// Maximum data length: 1024 bytes.
bool GPRS_send(byte* data, int len) {
 	char str[64] = ""; //String to gather answer from modem.
 	char c;
	unsigned long millis_start_of_send = millis();
 	unsigned long max_wait = 15000; //Maximum wait before time out.
	flushReg();
 	Serial3.print(F("AT+CIPSTART=\"TCP\",\"baatvaktserver.tele.ntnu.no\",\"8884\"")); //Connects to the server.
 	submit(0);
 	Serial3.flush(); // hold program until TX buffer is empty
	while (!cmdOK(str)) { //Waits until modem has confirmed that the command went through.
 		if (Serial3.available()) {
 			cstringAppend(str, (char)Serial3.read());
 		}
 		if ((unsigned long)millis() - millis_start_of_send >= max_wait) {
 			Serial.println("Timeout1 in GPRS_SEND");
 			return false; //Timeout
 		}
 	}
 	str[0] = '\0'; // reset cstring so we don't overflow
	while (!cmdOK(str)) { //Waits until we are fully connected to the modem.
 		if (Serial3.available()) {
 			cstringAppend(str, (char)Serial3.read());
 			if (connFailed(str)) { //If the connection failed we stop the sending.
 				return false;
 			}
 		}
 		if ((unsigned long)millis() - millis_start_of_send >= max_wait) {
 			Serial.println("Timeout2 in GPRS_SEND");
 			return false; //Time out.
 		}
 	}
 	str[0] = '\0';
	flushReg();
 	Serial3.print(F("AT+CIPSEND=")); // tell modem length of string. maximum length: 1024
 	Serial3.print(len);
 	submit(0);
 	while (!rdy2write(str)) { //Waits until the modem says its ready to be written to.
 		if (Serial3.available()) {
 			cstringAppend(str, (char)Serial3.read());
 		}
 		if ((unsigned long)millis() - millis_start_of_send >= max_wait) {
 			Serial.println("Timeout3 in GPRS_SEND");
 			return false; //Time out.
 		}
 	}
 	str[0] = '\0';
 	flushReg();
	Serial3.write(data, len); // pushes entire data buffer to Serial3
 	Serial3.write(26);
 	submit(0);
 	Serial3.flush();
	int dataCounter = 0;
 	while (!cmdOK(str)) {
 		if (Serial3.available()) {
 			dataCounter++;
 			if (dataCounter >= len) { // data buffer is echoed back. wait len chars before SEND OK
 				cstringAppend(str, (char)Serial3.read());
 			}
 			else {
 				Serial3.read(); // flush one char from RX buffer
 			}
 		}
 		if ((unsigned long)millis() - millis_start_of_send >= max_wait) {
 			Serial.println("Timeout4 in GPRS_SEND");
 			return false; //Time out.
 		}
 	}
 	str[0] = '\0';
 	Serial3.print(F("AT+CIPCLOSE")); //We close the connection to the server.
 	submit(0);
 	return true; //The data was sent, and we did not time out.
}


//We command the modem to give us its IMEI number, and returns it as a string of bytes.
//One byte for each digit because the number is longer than an unsigned long. 
//IMEI number is 15 digits long.

byte* get_IMEI_nr(){
	
	flushReg();
	Serial3.print(F("AT+GSN"));		//Asks modem for IMEI number
	submit(500);
	byte ret[15] = {};
	int counter = 0;
	byte c;
	while(Serial3.available()){
		c = Serial3.read();
		if( ((char)c >= '0' && (char)c <= '9') && counter < 15 ){
			ret[counter] = c;
			counter++;
		}
	}
	return ret;
}

//Pinging the given address.
bool GPRS_ping(char* adr){
	char t;
	char str[128] = "";
	flushReg();
	Serial3.print(F("AT+CIPPING=\""));	//Command to ping address.
	Serial3.print(adr);					//Printing address.
	Serial3.print("\"");
	submit(0);
	while(!cmdOK(str)){
		if(Serial3.available()){
			cstringAppend(str, (char)Serial3.read());
			
			if(cstringLength(str) >= 100){
				str[0] = '\0';
			}
			
			if(cmdError(str)){
				return false;
			}
		}

	}
	return true;
}

//Returns IP address assigned to the modem.
char* get_IP(){
	char str[64] = "";
	char t;
	int newLineCount = 0;
	flushReg();
	Serial3.print("AT+CIFSR");	//Get local IP address
	submit(0);
	while(newLineCount < 3){
		if(Serial3.available()){
			t = (char)Serial3.read();
			if( (t >= '0' && t <= '9') || t == '.'){
				cstringAppend(str, t);
			}
			if(t == 10){
				newLineCount++;
			}
		}
	}
	return str;
}

//Returns Unix time from SIM900
long int get_unix_ts(){
	char str[64] = "";
	char tmp[2] = "";
	flushReg();
	Serial3.print(F("AT+CCLK?"));	//Asks for local time
	submit(0);
	while(!cmdOK(str)){
		if(Serial3.available()){
			cstringAppend(str, (char)Serial3.read());
		}
	}

	int l = cstringLength(str);
	long int arr[6] = {};
	int arr_count = 0;
	
	//Converting time stamp from date time string to array
	for(int i = 29; i >= 0; i -= 3){
		tmp[0] = str[l-i];
		tmp[1] = str[l-i+1];
		arr[arr_count] = atol(tmp);
		arr_count++;
		if(arr_count >= 6){
			break;
		}
	}
	arr[0] += 2000; //15 + 2000 = 2015
	
	//Using Time.h library to convert from date time to Unix time
	tmElements_t tmSet;
	
	tmSet.Year = arr[0] - 1970;
	tmSet.Month = arr[1];		
	tmSet.Day = arr[2];
	tmSet.Hour = arr[3];
	tmSet.Minute = arr[4];
	tmSet.Second = arr[5];
	
	return makeTime(tmSet);         //Return Unix time as unsigned long
}


bool NTP_sync(){
	char str[64] = "";
	flushReg();
	Serial3.print(F("AT+CNTP"));	//Synchronize with the NTP server we connected to in GPRS_setup()
	submit(0);
	while(!cmdOK(str)){
		if(Serial3.available()){
			cstringAppend(str, (char)Serial3.read());
		}
	}

	return true;
}


//Empties the receive buffer 
void flushReg(){
	while(Serial3.available() > 0){
		char t = (char)Serial3.read();
	}
	return;
}

//Writes ASCII signs with values 13 (submit) and 10 (newline) and creates delay. 
void submit(uint16_t time){
	if(time>=0){
		Serial3.write(13);   //This combination is the same as hitting the enter key once. 
		Serial3.write(10);
		if(time>0){
			delay(time); //In-parameter is delay. 			
		}

	}
	return;
}








