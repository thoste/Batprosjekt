void printDate(){
	Serial.print("Hour: ");
	Serial.print(hour());
	Serial.print("\t Minute: ");
	Serial.print(minute());
	Serial.print("\t Second: ");
	Serial.print(second());
	Serial.print("\t Day: ");
	Serial.print(day());
	Serial.print("\t Month: ");
	Serial.print(month());
	Serial.print("\t Year: ");
	Serial.println(year());
}
