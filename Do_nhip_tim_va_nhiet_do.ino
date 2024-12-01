
/*  Getting_BPM_to_Monitor prints the BPM to the Serial Monitor, using the least lines of code and PulseSensor Library.
 *  Tutorial Webpage: https://pulsesensor.com/pages/getting-advanced
 *
--------Use This Sketch To------------------------------------------
1) Displays user's live and changing BPM, Beats Per Minute, in Arduino's native Serial Monitor.
2) Print: "♥  A HeartBeat Happened !" when a beat is detected, live.
2) Learn about using a PulseSensor Library "Object".
4) Blinks LED on PIN 13 with user's Heartbeat.
--------------------------------------------------------------------*/
#include "DHT.h"

#define DHTPIN A1     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);
#include "Wire.h"
#include "Adafruit_LiquidCrystal.h"

// initialize the library with the numbers of the interface pins
Adafruit_LiquidCrystal lcd(4, 3, 5,6,7,8);
/* LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2*/
 
#include <SoftwareSerial.h>
SoftwareSerial Ser(9, 10);
int myBPM =60;
float temp;
#include <Wire.h>
#include "MAX30105.h"

#include "heartRate.h"

MAX30105 particleSensor;

const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;
long times;
#define coi 11
#define relay 12
void setup() {   
  dht.begin();
lcd.begin(16, 2);
  pinMode(coi, OUTPUT);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);
  Serial.begin(9600);          // For Serial Monitor
  Ser.begin(9600);
  delay(1000);
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  Serial.println("Place your index finger on the sensor with steady pressure.");

  particleSensor.setup(); //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED
}



void loop() {
  

  read_BMP();
  
  if(millis() - times >= 1100){
    read_temp();
  String data = (String(temp) + "," + String(myBPM));
  Ser.println(data);
  Serial.println(data);
  times = millis();
  }
}
void read_BMP(){
  long irValue = particleSensor.getIR();

  if (checkForBeat(irValue) == true)
  {
    //We sensed a beat!
    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20)
    {
      rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
      rateSpot %= RATE_SIZE; //Wrap variable

      //Take average of readings
      beatAvg = 0;
      for (byte x = 0 ; x < RATE_SIZE ; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
  }

  Serial.print("IR=");
  Serial.print(irValue);
  Serial.print(", BPM=");
  Serial.print(beatsPerMinute);
  Serial.print(", Avg BPM=");
  Serial.print(beatAvg);
  myBPM = beatsPerMinute;
  if(myBPM >= 100 || myBPM < 60){
    digitalWrite(coi, HIGH);
    
  }
  else digitalWrite(coi, LOW);
  if(temp >= 40 || temp < 15){
    digitalWrite(relay, LOW);
  }
  else digitalWrite(relay, HIGH);
  
  if (irValue < 50000)
    Serial.print(" No finger?");

  Serial.println();
}
void read_temp()
{
  float t = dht.readTemperature();
  temp =  t;
  Serial.print("Temperature:");
  Serial.println(temp);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("BPM :");
  lcd.setCursor(7, 0);
  lcd.print(myBPM);
  lcd.setCursor(0, 1);
  lcd.print("Temp.:");
  lcd.setCursor(7, 1);
  lcd.print(temp);
  lcd.setCursor(13, 1);
  lcd.print(" C");
}
  
