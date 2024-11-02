#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_MCP9808.h>


// put variable declarations here:
#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme1; //0x76 sdo vdd na 0x77
Adafruit_BME280 bme2; //0x77
float bme1_temperature;
float bme1_humidity;
float bme1_pressure;
float bme1_altitude;
float bme2_temperature;
float bme2_humidity;
float bme2_pressure;
float bme2_altitude;
Adafruit_MCP9808 mcp1; //0x18, a0 vdd na 0x19
Adafruit_MCP9808 mcp2; //0x19
float mcp1_temperature;
float mcp2_temperature;


// put function declarations here:
void BME1getValues();
void BME1printValues();
void MCP1getValues();
void MCP1printValues();
void BME2getValues();
void BME2printValues();
void MCP2getValues();
void MCP2printValues();

void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  Serial.print("Start");
  // test dioda
  pinMode(LED_BUILTIN, OUTPUT);
  // default settings BME
  if (!bme1.begin(0x76)) {
    Serial.println("Could not find a valid BME280x76 sensor!");
  } else {
    Serial.println("BME280x76 connected");
    Serial.println();
  }
  if (!bme2.begin(0x77)) {
    Serial.println("Could not find a valid BME280x77 sensor!");
  } else {
    Serial.println("BME280x77 connected");
    Serial.println();
  }
  if (!mcp1.begin(0x18)) {
    Serial.println("Could not find a valid MCP9808x18 sensor!");
  } else {
    Serial.println("MCP9808x18 connected");
    Serial.println();
  }
  if (!mcp2.begin(0x19)) {
    Serial.println("Could not find a valid MCP9808x19 sensor!");
  } else {
    Serial.println("MCP9808x19 connected");
    Serial.println();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, HIGH);
  delay(5000);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("===========new readings=============");
  Serial.println("======room 1=======");
  BME1getValues();
  BME1printValues();
  MCP1getValues();
  MCP1printValues();
  Serial.println("======room 2=======");
  BME2getValues();
  BME2printValues();
  MCP2getValues();
  MCP2printValues();
  Serial.println("====================================");
  delay(5000);
}


// put function definitions here:
void BME1getValues() {
  bme1_temperature = bme1.readTemperature();
  bme1_pressure = bme1.readPressure() / 100.0F;
  bme1_altitude = bme1.readAltitude(SEALEVELPRESSURE_HPA);
  bme1_humidity = bme1.readHumidity();
}

void BME1printValues() {
  Serial.print("BME Temperature = ");
  Serial.print(bme1_temperature);
  Serial.println(" °C");

  Serial.print("BME Pressure = ");
  Serial.print(bme1_pressure);
  Serial.println(" hPa");

  Serial.print("BME Approx. Altitude = ");
  Serial.print(bme1_altitude);
  Serial.println(" m");

  Serial.print("BME Humidity = ");
  Serial.print(bme1_humidity);
  Serial.println(" %");
  Serial.println();
}

void BME2getValues() {
  bme2_temperature = bme2.readTemperature();
  bme2_pressure = bme2.readPressure() / 100.0F;
  bme2_altitude = bme2.readAltitude(SEALEVELPRESSURE_HPA);
  bme2_humidity = bme2.readHumidity();
}

void BME2printValues() {
  Serial.print("BME Temperature = ");
  Serial.print(bme2_temperature);
  Serial.println(" °C");

  Serial.print("BME Pressure = ");
  Serial.print(bme2_pressure);
  Serial.println(" hPa");

  Serial.print("BME Approx. Altitude = ");
  Serial.print(bme2_altitude);
  Serial.println(" m");

  Serial.print("BME Humidity = ");
  Serial.print(bme2_humidity);
  Serial.println(" %");
  Serial.println();
}

void MCP1getValues() {
  mcp1.wake();
  mcp1_temperature = mcp1.readTempC();
  mcp1.shutdown_wake(1);
}
void MCP1printValues() {
  Serial.println();
  Serial.print("MCP Temperature = ");
  Serial.print(mcp1_temperature);
  Serial.println(" °C");
  Serial.println();
}

void MCP2getValues() {
  mcp2.wake();
  mcp2_temperature = mcp2.readTempC();
  mcp2.shutdown_wake(1);
}
void MCP2printValues() {
  Serial.println();
  Serial.print("MCP Temperature = ");
  Serial.print(mcp2_temperature);
  Serial.println(" °C");
  Serial.println();
}