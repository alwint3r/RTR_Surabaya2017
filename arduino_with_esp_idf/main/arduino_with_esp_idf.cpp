#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <Adafruit_BMP085.h>
#include <PubSubClient.h>
#include <WiFi.h>

#define MQTT_HOST "test.mosquitto.org"
#define MQTT_CLIENT_ID "bmp180nano32"
#define WIFI_SSID ""
#define WIFI_PASS ""

WiFiClient wifiClient;
PubSubClient mqtt(MQTT_HOST, 1883, wifiClient);

void blink_task(void* args);
void bmp180_task(void* args);
void publish_data_task(void* args);
void reconnectMqtt();

Adafruit_BMP085 bmp;
static bool isBMPInitialized = false;

extern "C" void app_main() {
    initArduino();
    Serial.begin(115200);
    delay(3000);

    xTaskCreate(blink_task, "blink_task", 2048, NULL, 5, NULL);
    xTaskCreate(bmp180_task, "bmp180_task", 2048, NULL, 5, NULL);
    xTaskCreate(publish_data_task, "publish_data_task", 8192, NULL, 5, NULL);
}

void reconnectMqtt() {
    while (!mqtt.connected()) {
        if (mqtt.connect(MQTT_CLIENT_ID)) {
            break;
        } else {
            delay(1000);
        }
    }
}

void publish_data_task(void* args) {
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    while (WiFi.status() != WL_CONNECTED) {
        Serial.println("Still waiting for WiFi connection...");
        delay(1000);
    }

    Serial.println("Initializing MQTT connection");
    reconnectMqtt();

    Serial.println("Will start publishing temperature data");

    while (true) {
        if (!isBMPInitialized) {
            delay(500);
            continue;
        }

        if (!mqtt.connected()) {
            Serial.println("MQTT is reconnecting");
            reconnectMqtt();
        }

        Serial.println("Sending temperature data...");
        mqtt.publish("alwin/bmp180nano32/temperature", String(bmp.readTemperature()).c_str());

        delay(3000);
    }
}

void blink_task(void* args) {
    int LED_PIN = 16;
    int level = 0;

    Serial.println("Starting blink_task");
    pinMode(LED_PIN, OUTPUT);

    while (true) {
        Serial.println("Blinking.");
        digitalWrite(LED_PIN, level);
        level = !level;
        delay(500);
    }
}

void bmp180_task(void* args) {
    Serial.println("Starting bmp180_task");
    if (!bmp.begin()) {
        vTaskDelete(NULL);
    }

    isBMPInitialized = true;

    while (true) {
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

        Serial.print("Real altitude = ");
        Serial.print(bmp.readAltitude(101500));
        Serial.println(" meters");

        delay(3000);
    }
}