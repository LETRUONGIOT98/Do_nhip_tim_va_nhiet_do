
#define BLYNK_TEMPLATE_ID "TMPLXDwzeBM4"
#define BLYNK_TEMPLATE_NAME "HO CA"

#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

// Uncomment your board, or configure a custom board in Settings.h
//#define USE_SPARKFUN_BLYNK_BOARD
#define USE_NODE_MCU_BOARD
//#define USE_WITTY_CLOUD_BOARD
//#define USE_WEMOS_D1_MINI
#include <SoftwareSerial.h>
SoftwareSerial Ser(D5, D6);
#include "BlynkEdgent1.h"
String DATA;

void setup()
{
  Serial.begin(115200);
  delay(100);
  Ser.begin(9600);
  BlynkEdgent.begin();
}

void loop() {
 BlynkEdgent.run();  //Khởui động blynk
  if(Ser.available() >0){
    DATA = Ser.readString();
    //Serial.println(DATA);
    String IN1 = getValue(DATA,',',0);
    String IN2 = getValue(DATA,',',1);
    float temp = IN1.toFloat();
    float BPM =IN2.toFloat();
     Blynk.virtualWrite(V0,temp);
     Blynk.virtualWrite(V1,BPM );
}
}
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
