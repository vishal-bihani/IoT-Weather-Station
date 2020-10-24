
#include <DHT.h>
#include <ESP8266WiFi.h>
#include<Wire.h>
#include<Adafruit_BMP085.h>

Adafruit_BMP085 bmp;                // Pressure Sensor

// API KEY //
String key = "EFYMIKU1DAXPJ3XH";
char ssid[] = "vishal";             // WiFi SSID
char pass[] = "password";           // WiFi Password

const char* api = "api.thingspeak.com";     // ThingSpeak Server API
#define DHTPIN D0 

DHT dht(DHTPIN, DHT11, 15);         // DHT11 Initialization
 
WiFiClient node;

void setup() {
  Serial.begin(115200);
  dht.begin();
  bmp.begin();

  WiFi.begin(ssid, pass);
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  node.connect(api, 80);
}

void loop() {

  // Measuring Values //
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  float pressure = bmp.readPressure();
  int rainReading = analogRead(A0);
  int range = map(rainReading, 0, 1023, 0, 3);


  // Transferring Values //
  if (node.connected()) {

    // Constructing the api body //
    String httpPost = key;
    httpPost +="&field1=";
    httpPost += String(temperature);
    httpPost +="&field2=";
    httpPost += String(humidity);
    httpPost += "&field3=";
    httpPost += String(pressure);
    httpPost += "&field4=";
    httpPost += String(range);
    httpPost += "\r\n\r\n";

    // Sending the values //
    node.print("POST /update HTTP/1.1\n");
    node.print("Host: api.thingspeak.com\n");
    node.print("Connection: close\n");
    node.print("X-THINGSPEAKAPIKEY: "+key+"\n");
    node.print("Content-Type: application/x-www-form-urlencoded\n");
    node.print("Content-Length: ");
    node.print(httpPost.length());
    node.print("\n\n");
    node.print(httpPost);

    // Printing on Serial Monitor //
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" degrees Celcius Humidity: ");
    Serial.print(humidity);
    Serial.print(" Pressure : ");
    Serial.println(pressure);
  }
  
  delay(15000);       // 15 secs delay for thingspeak to get updated.
}
