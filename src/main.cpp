#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <AntaresESPMQTT.h>

#define SSID "AHDA 4434"
#define PASS "Ahda@hotspot"
#define mqtt_server "mqtt.antares.id" // Using antares mqtt broker

#define ACCESSKEY "5857af4ec372ee95:d546da27532c4a59" // Antares account access key
#define projectName "tracker"                         // Name of the application created in Antares
#define deviceName "esp32"                            // Name of the device created in Antares

AntaresESPMQTT antares(ACCESSKEY);

void publish()
{
  Serial.println("\n====");
  int temp = random(25, 30);
  int hum = random(75, 90);
  antares.add("temperature", temp);
  antares.add("humidity", hum);
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

  Serial.println("\nConncting to antares MQTT..");
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
  delay(15000);
}