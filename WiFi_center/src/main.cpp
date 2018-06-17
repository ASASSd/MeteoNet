#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
SoftwareSerial bluetooth(5, 4);
int temp, hum, temp1=0, hum1=0;
WiFiServer server(80);
int maxtemp=-9999999,  maxhum=-9999999, mintemp=9999999, minhum=9999999;

void setup(){
  Serial.begin(9600);
  Serial.println();
  bluetooth.begin(38400);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.printf("Connecting to WIFI ");
  WiFi.begin("Keenetic-8636", "nVkcBZh3");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");
  server.begin();
  Serial.printf("Web server started, IP is %s ", WiFi.localIP().toString().c_str());
  digitalWrite(LED_BUILTIN, LOW);
  //WiFi.softAPdisconnect(true);
}



String prepareHtmlPage()
{
  String htmlPage =
     String("HTTP/1.1 200 OK\r\n") +
            "Content-Type: text/html\r\n" +
            "Connection: close\r\n" +
            "refresh: 2\r\n" +
            "\r\n" +
            "<!DOCTYPE HTML>" +
            "<html>" +
            "<head>" +
            "<script type=" + '"' + "text/javascript" + '"' + '>'  + " setTimeout(function(){   location.reload(); }, 2000);   </script>"
            "</head>" +
            "<body>" +
            "Temperature :  " + temp1 + "&#186C" + "<br />" +
            "Humidity : " + hum1 + '%' + "<br />" +
            "Max registered temperature : " + maxtemp + "&#186C" + "<br />" +
            "Min registered temperature : " + mintemp + "&#186C" + "<br />" +
            "Max registered humidity : " + maxhum + '%' + "<br />" +
            "Min registered humidity : " + minhum + '%' + "<br />" +
            "</body>" +
            "</html>" +
            "\r\n";
  return htmlPage;
}

void getVals(){
  String data = bluetooth.readString();
  for(int i=0; i<(int)data.length(); i++){
    if(data[i]=='t'){
      temp=(10*(data[i+1]-'0'))+data[i+2]-'0';
    }
    if(data[i]=='h'){
      hum=(10*(data[i+1]-'0'))+data[i+2]-'0';
    }
  }
  //temp=bluetooth.parseInt();
  //hum=bluetooth.parseInt();
  if((temp1 != temp || hum1 != hum) && (temp != 0) && (hum != 0)){
    temp1=temp;
    hum1=hum;
}
}

void maxMinValsDetect(){
  if(hum != 0 && temp != 0){
  if(hum > maxhum){
    maxhum=hum;
  }
  if(temp >= maxtemp){
    maxtemp=temp;
  }
  if(hum < minhum){
    minhum=hum;
  }
  if(temp < mintemp){
    mintemp=temp;
  }
}
}

void loop()
{
  getVals();
  maxMinValsDetect();
  WiFiClient client = server.available();
  if (client)
  {
    Serial.println("\n[Client connected]");
    while (client.connected())
    {
      if (client.available())
      {
        String line = client.readStringUntil('\r');
        Serial.print(line);
        if (line.length() == 1 && line[0] == '\n')
        {
          digitalWrite(LED_BUILTIN, HIGH);
          client.println(prepareHtmlPage());
          digitalWrite(LED_BUILTIN, LOW);
          break;
        }
      }
    }
    Serial.println(hum);
    Serial.print(temp);
    delay(2000);
    client.stop();
    Serial.println("[Client disconnected]");
  }

}
