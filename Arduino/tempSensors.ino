void tempSensors(){
	// call sensors.requestTemperatures() to issue a global temperature
  	// request to all devices on the bus
  	// Serial.print(" Requesting temperatures...");
  	sensors.requestTemperatures(); // Send the command to get temperatures
  	// Serial.println("DONE");

  	temp1 = sensors.getTempCByIndex(0);
  	temp2 = sensors.getTempCByIndex(1);
  	tempM = (temp1 + temp2)/2;
  	// Why "byIndex"? 
    // You can have more than one IC on the same bus. 
    // 0 refers to the first IC on the wire
  
  	Serial.println("Temperatures :\t");
  	Serial.print("T1: ");
  	Serial.print(temp1);
  	Serial.print("\t");
  	Serial.print("T2: ");
  	Serial.print(temp2);
  	Serial.print("\t");
  	Serial.print("mean: ");
  	Serial.println(tempM);
}