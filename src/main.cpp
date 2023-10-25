#include <Arduino.h>
#include <AntaresESP32MQTT.h>
#include <DHTesp.h>

#define SSID "MBC-Lab 2.4G"
#define PASS "gogombc123"
#define mqtt_server "mqtt.antares.id" // Using antares mqtt broker

#define ACCESSKEY "5857af4ec372ee95:d546da27532c4a59" // Antares account access key
#define projectName "tracker"                         // Name of the application created in Antares
#define deviceName "esp32"                            // Name of the device created in Antares

const int dht = 13; // Pin on ESP32

DHTesp dhtsensor;
AntaresESP32MQTT antares(ACCESSKEY);

void publish()
{
  Serial.println("\n====");
  Serial.println("[Antares] Publishing.");
  int temp = random(25, 30); // Testing purpose only
  int hum = random(75, 90);  // Testing purpose only

  TempAndHumidity data = dhtsensor.getTempAndHumidity();

  Serial.print("[Antares] Temperature : ");
  Serial.println(data.temperature);
  Serial.print("[Antares] Humidity    : ");
  Serial.println(data.humidity);

  antares.add("temperature", data.temperature);
  antares.add("humidity", data.humidity);
  antares.publish(projectName, deviceName);
  Serial.println("=== Published ===\n");
  delay(500);
}

void setup()
{
  Serial.begin(115200);
  delay(500);

  Serial.println("\n[DHT-22] Prepairing DHT sensor...");
  dhtsensor.setup(dht, DHTesp::DHT22);
  Serial.println("\n[DHT-22] Done.");

  /*
    setDebug function is broken and to use the debug function you need to change the code inside
      .pio\libdeps\esp32dev\Antares ESP32 MQTT\src\AntaresESP32MQTT.cpp
    on line 208 and add the following return statement

    "return trueFalse;"
  */
  antares.setDebug(true); // Currently broken because of antares repo
  antares.wifiConnection(SSID, PASS);
  antares.setMqttServer();
}

void loop()
{
  antares.checkMqttConnection();

  publish();

  delay(2000);
}