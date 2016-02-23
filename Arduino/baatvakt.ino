#include <Wire.h>
#include <TimerOne.h>
#include <OneWire.h>
#include <DallasTemperature.h>
 

#define ONE_WIRE_BUS 2
OneWire Bus(ONE_WIRE_BUS);

DallasTemperature sensors(&Bus);

int sTime = 3000;

void setup()
{
	Serial.begin(9600);

	sensors.begin();
}

int i = 0;
double temps1[11];
double temps2[11];

void loop()
{
	sensors.requestTemperatures();

	temps1[i] = sensors.getTempCByIndex(0);
	temps2[i] = sensors.getTempCByIndex(1);

	i++;
	if (i == 11) {
		i = 0;
	}


	Serial.println(middle(temps1, 11));
	Serial.println(middle(temps2, 11));

	delay(100);

}