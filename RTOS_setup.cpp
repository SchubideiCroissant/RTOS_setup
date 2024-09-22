#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"

void vTask1(void *pvParameters) {
    while (true) {
        printf("Task 1 is running!\n");
        vTaskDelay(pdMS_TO_TICKS(1000));  // 1 Sekunde Pause
    }
}

void vTask2(void *pvParameters) {
    while (true) {
        printf("Task 2 is running!\n");
        vTaskDelay(pdMS_TO_TICKS(2000));  // 2 Sekunden Pause
    }
}

int main() {
    stdio_init_all();

    // Erstelle Task 1
    xTaskCreate(vTask1, "Task 1", 256, NULL, 1, NULL);

    // Erstelle Task 2
    xTaskCreate(vTask2, "Task 2", 256, NULL, 1, NULL);

    // Starte den Scheduler
    vTaskStartScheduler();

    while (true) {
        // Sollte nie erreicht werden
    }
}
