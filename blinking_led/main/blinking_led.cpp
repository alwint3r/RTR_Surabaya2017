#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>

void blinkingTask(void* parameters) {
    gpio_num_t LED_PIN = GPIO_NUM_16;
    int level = 0;

    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    while (true) {
        printf("Blinking!\n");
        gpio_set_level(LED_PIN, level);
        level = !level;
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

extern "C" void app_main() {
    xTaskCreate(blinkingTask, "blinkingTask", 2048, NULL, 5, NULL);
}