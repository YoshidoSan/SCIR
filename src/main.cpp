#include "constants.h"

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_MCP9808.h>

#include <ThingSpeak.h>
#include <WiFi.h>
#include <Wire.h>

#include <esp_sleep.h>
#include <vector>

WiFiClient client;

#define SEALEVELPRESSURE_HPA (1013.25)
#define DEBUGGING true

float bme1_temperature = 0.0, bme1_humidity = 0.0, bme1_pressure = 0.0, bme1_altitude = 0.0;
float bme2_temperature = 0.0, bme2_humidity = 0.0, bme2_pressure = 0.0, bme2_altitude = 0.0;
float mcp1_temperature = 0.0, mcp2_temperature = 0.0;

Adafruit_BME280 bme1; // 0x76
Adafruit_BME280 bme2; // 0x77
Adafruit_MCP9808 mcp1; // 0x18
Adafruit_MCP9808 mcp2; // 0x19

std::vector<float> BMEgetValues(Adafruit_BME280& bme) {
  float temperature = bme.readTemperature();
  float pressure = bme.readPressure() / 100.0F;
  float altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  float humidity = bme.readHumidity();
  return {temperature, pressure, humidity, altitude};
}

float MCPgetValues(Adafruit_MCP9808& mcp) {
  mcp.wake();
  float temperature = mcp.readTempC();
  mcp.shutdown_wake(1);
  return temperature;
}

void BMEprintValues(int bme_number, float temperature, float pressure, float humidity, float altitude) {
  Serial.print("BME number: ");
  Serial.println(bme_number);

  Serial.print("BME Temperature = ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("BME Pressure = ");
  Serial.print(pressure);
  Serial.println(" hPa");

  Serial.print("BME Humidity = ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("BME Approx. Altitude = ");
  Serial.print(altitude);
  Serial.println(" m");
  Serial.println();
}

void MCPprintValues(int mcp_number, float temperature) {
  Serial.print("MCP number: ");
  Serial.println(mcp_number);

  Serial.print("MCP Temperature = ");
  Serial.print(temperature);
  Serial.println(" °C");
  Serial.println();
}

void sent_data() {
  ThingSpeak.setField(1, bme1_temperature);
  ThingSpeak.setField(2, bme1_humidity);
  ThingSpeak.setField(3, bme1_pressure);

  ThingSpeak.setField(4, bme2_temperature);
  ThingSpeak.setField(5, bme2_humidity);
  ThingSpeak.setField(6, bme2_pressure);

  ThingSpeak.setField(7, mcp1_temperature);
  ThingSpeak.setField(8, mcp2_temperature);

  int response = ThingSpeak.writeFields(channelID, writeAPIKey);

  if (DEBUGGING) {
    if (response == 200) {
      Serial.println("Dane wysłane do ThingSpeak!");
    } else {
      Serial.print("Błąd wysyłania danych, kod błędu: ");
      Serial.println(response);
    }
  }
}

void collect_sensor_data() {
  auto bme1_values = BMEgetValues(bme1);
  bme1_temperature = bme1_values[0];
  bme1_pressure = bme1_values[1];
  bme1_humidity = bme1_values[2];
  bme1_altitude = bme1_values[3];

  auto bme2_values = BMEgetValues(bme2);
  bme2_temperature = bme2_values[0];
  bme2_pressure = bme2_values[1];
  bme2_humidity = bme2_values[2];
  bme2_altitude = bme2_values[3];

  mcp1_temperature = MCPgetValues(mcp1);
  mcp2_temperature = MCPgetValues(mcp2);

  sent_data();

  if (DEBUGGING) {
    BMEprintValues(1, bme1_temperature, bme1_pressure, bme1_humidity, bme1_altitude);
    BMEprintValues(2, bme2_temperature, bme2_pressure, bme2_humidity, bme2_altitude);
    MCPprintValues(1, mcp1_temperature);
    MCPprintValues(2, mcp2_temperature);
  }
}

void setup() {
  Serial.begin(115200);
  delay(2000);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Łączenie z WiFi...");
  }
  Serial.println("Połączono z WiFi!");

  ThingSpeak.begin(client);

  if (!bme1.begin(0x76)) Serial.println("BME280 (0x76) not found!");
  if (!bme2.begin(0x77)) Serial.println("BME280 (0x77) not found!");
  if (!mcp1.begin(0x18)) Serial.println("MCP9808 (0x18) not found!");
  if (!mcp2.begin(0x19)) Serial.println("MCP9808 (0x19) not found!");

  if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER) {
    collect_sensor_data();
  }

  esp_sleep_enable_timer_wakeup(30 * 1000000); // 30s
  esp_deep_sleep_start();
}

void loop() {}