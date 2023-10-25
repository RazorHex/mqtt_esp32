#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <AntaresESPMQTT.h>
#include <DHTesp.h>

#define SSID "MBC-Lab 2.4G"
#define PASS "gogombc123"
#define mqtt_server "mqtt.antares.id" // Using antares mqtt broker

#define ACCESSKEY "5857af4ec372ee95:d546da27532c4a59" // Antares account access key
#define projectName "tracker"                         // Name of the application created in Antares
#define deviceName "esp32"                            // Name of the device created in Antares

const int dht = 13;

DHTesp dhtsensor;
AntaresESPMQTT antares(ACCESSKEY);

void publish()
{
  Serial.println("\n====");
  int temp = random(25, 30);
  int hum = random(75, 90);

  TempAndHumidity data = dhtsensor.getTempAndHumidity();

  antares.add("temperature", data.temperature);
  antares.add("humidity", data.humidity);
  antares.publish(projectName, deviceName);
  Serial.println("=== Published ===\n");
}

void callback(char topic[], byte payload[], unsigned int length)
{
  Serial.println("\n=====");
  antares.get(topic, payload, length);

  Serial.println(antares.getTopic());
  Serial.println("Payload: " + antares.getPayload());
}

void setup()
{
  Serial.begin(115200);
  delay(500);

  Serial.println("\n[DHT-22] Prepairing DHT sensor...");
  dhtsensor.setup(dht, DHTesp::DHT22);
  Serial.println("\n[DHT-22] Done.");

  antares.setDebug(true);
  antares.wifiConnection(SSID, PASS);
  antares.setMqttServer();
  antares.setCallback(callback);
}

void loop()
{

  Serial.println("Checking connection...");
  antares.checkMqttConnection();

  publish();

  delay(500);
  antares.retrieveLastData(projectName, deviceName);
  delay(10000);
}