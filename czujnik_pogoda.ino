#include <BH1750.h>
#include <Wire.h>
#include <WiFiS3.h>

AM2320_asukiaaa temp_hum_sensor;
BH1750 lightMeter;
int rainSensor = A0;
int threshold = 500;
char ssid[] = " ";        
char pass[] = " ";    
char server[] = " ";
int port = 80;
WiFiClient client;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected to WiFi. IP address: ");
  Serial.println(WiFi.localIP());
  lightMeter.begin();
  temp_hum_sensor.setWire(&Wire);
  pinMode(rainSensor, INPUT);

}

void loop() {
  temp_hum_sensor.update();
  float lux = lightMeter.readLightLevel();
  float temperature = temp_hum_sensor.temperatureC;
  float humidity = temp_hum_sensor.humidity;
  int rain = analogRead(rainSensor);
  String measurments = String(temperature) +"-"+ String(humidity) +"-"+ String(rain)+"-"+String(lux);
  Serial.println(measurments);
  Serial.println("");
  sendRequest
  delay(1000);

  // put your main code here, to run repeatedly:

}

void sendRequest(String measurments){
  if (client.connect(server, port)) 
  {
      Serial.println("Connected to server");
      String postData = "{\"value\": " + measurments + "}";
      client.println("POST /measures HTTP/1.1");
      client.println("Host:  ");
      client.println("Accept: application/json");
      client.println("Content-Type: application/json");
      client.print("Content-Length: ");
      client.println(postData.length());
      client.println();
      client.println(postData);

      while (client.connected() || client.available()) {
        delay(1000);
        if (client.available()) {
          String line = client.readStringUntil('\n');
          Serial.println(line);
        }
        else{break;}
      }
      
      client.stop();
      Serial.println("Disconnected from server");
      delay(1000);
  } 
  else 
  {
    Serial.println("Connection to server failed");
  }
}
