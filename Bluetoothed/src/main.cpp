#include <Arduino.h>
#include <SoftwareSerial.h>
#include <DHT.h>
DHT dht(4, DHT11);
SoftwareSerial bluetooth(9, 8);
float humid;
float temp;
double maxtemp=-9999999,  maxhum=-9999999, mintemp=9999999, minhum=9999999;
float lastTemp, lastHumid;
bool flagh=false, flagt=false;
void setup(){
    bluetooth.begin(9600);
    //Serial.begin(9600);
}
/*
void corruptVal(){
  bluetooth.print("Unable to get info - sensor error");
  bluetooth.println();
}

void maxminStatout(){
  bluetooth.print("Max recorded temperature : ");
  bluetooth.println(maxtemp);
  bluetooth.print("Max recorded humidity : ");
  bluetooth.println(maxhum);
  bluetooth.print("Min recorded temperature : ");
  bluetooth.println(mintemp);
  bluetooth.print("Min recorded humidity : ");
  bluetooth.println(minhum);
}

void sendVals(){
  bluetooth.println(" ");
  bluetooth.println(" ");
  bluetooth.print("Temperature is ");
  bluetooth.print(temp);
  bluetooth.print(" and humidity is ");
  bluetooth.println(humid);
  maxminStatout();
}

void maxMinValsDetect(){
  if(humid > maxhum){
    maxhum=(double)humid;
  }
  if(temp >= maxtemp){
    maxtemp=(double)temp;
  }
  if(humid < minhum){
    minhum=(double)humid;
  }
  if(temp < mintemp){
    mintemp=(double)temp;
  }
}
*/
void espDataOut(){
  bluetooth.print('t');
  if(flagh==true){bluetooth.print(0);}
  bluetooth.print((int)temp);
  bluetooth.print('h');
  if(flagt==true){bluetooth.print(0);}
  bluetooth.println((int)humid);
}

void loop(){
humid = dht.readHumidity();
temp = dht.readTemperature();
//maxMinValsDetect();
if(humid < 10){flagh=true;}
if(temp < 10){flagt=true;}
if(isnan(humid) || isnan(temp)){
  //corruptVal();
  return;
}else if(lastTemp != temp || lastHumid != humid){
  //sendVals();
  espDataOut();
  lastTemp=temp;
  lastHumid=humid;
}
delay(2000);
flagh=false;
flagt=false;
}
