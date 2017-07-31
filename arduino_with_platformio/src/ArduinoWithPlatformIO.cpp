#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>

#define SDA_PIN 21
#define SCL_PIN 22

Adafruit_BMP085 bmp;

void setup() {
    Serial.begin(9600);
    delay(3000);
    Serial.println("Initializing.");

    if (!bmp.begin()) {
        Serial.println("Could not find a valid BMP085 sensor, check wiring!");
        while (1) {}
    }
}

void loop() {
    Serial.print("Temperature = ");
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");
    
    Serial.print("Pressure = ");
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");

    Serial.print("Altitude = ");
    Serial.print(bmp.readAltitude());
    Serial.println(" meters");

    Serial.print("Pressure at sealevel (calculated) = ");
    Serial.print(bmp.readSealevelPressure());
    Serial.println(" Pa");
    
    Serial.println();
    delay(1500);
}
