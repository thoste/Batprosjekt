void modemSetup(){
  //Modem setup
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
}

