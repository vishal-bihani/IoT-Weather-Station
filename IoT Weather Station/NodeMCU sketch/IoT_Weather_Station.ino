
#include <DHT.h>
#include <ESP8266WiFi.h>
#include<Wire.h>
#include<Adafruit_BMP085.h>

Adafruit_BMP085 bmp;

String key = "EFYMIKU1DAXPJ3XH";
char ssid[] = "vishal";
char pass[] = "password";

const char* api = "api.thingspeak.com";
#define DHTPIN 2 

DHT dht(DHTPIN, DHT11, 15); 
WiFiClient node;

void setup() {
  Serial.begin(115200);
  dht.begin();
  bmp.begin();

  WiFi.begin(ssid, pass);
  
  Serial.println("Connecting...");

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  node.connect(api, 80);
}

void loop() {

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  float pressure = bmp.readPressure();

  if (node.connected()) { 
    String httpPost = key;
    httpPost +="&field1=";
    httpPost += String(temperature);
    httpPost +="&field2=";
    httpPost += String(humidity);
    httpPost += "\r\n\r\n";

    node.print("POST /update HTTP/1.1\n");
    node.print("Host: api.thingspeak.com\n");
    node.print("Connection: close\n");
    node.print("X-THINGSPEAKAPIKEY: "+key+"\n");
    node.print("Content-Type: application/x-www-form-urlencoded\n");
    node.print("Content-Length: ");
    node.print(httpPost.length());
    node.print("\n\n");
    node.print(httpPost);

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" degrees Celcius Humidity: ");
    Serial.print(humidity);
    Serial.println("% send to Thingspeak");
  }
  
  delay(3000);
}
