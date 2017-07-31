/* ADC1 Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "driver/adc.h"

#define ADC1_TEST_CHANNEL (ADC1_CHANNEL_6)

QueueHandle_t LDRQueue;

void readLDRData(void* arg) {
    // initialize ADC
    adc1_config_width(ADC_WIDTH_12Bit);
    adc1_config_channel_atten(ADC1_TEST_CHANNEL, ADC_ATTEN_11db);
    int data;

    while (true) {
        data = adc1_get_voltage(ADC1_TEST_CHANNEL);

        xQueueSendToBack(LDRQueue, &data, portMAX_DELAY);
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}

void processData(void* arg) {
    int data;

    while (true) {
        xQueueReceive(LDRQueue, &data, portMAX_DELAY);
        printf("LDR value: %d\n", data);

        // Do process the data from LDR sensor
    }
}

extern "C" void app_main() {
    LDRQueue = xQueueCreate(sizeof(int), 10);

    xTaskCreate(readLDRData, "readLDRData", 1024*3, NULL, 5, NULL);
    xTaskCreate(processData, "processData", 2048, NULL, 5, NULL);
}

