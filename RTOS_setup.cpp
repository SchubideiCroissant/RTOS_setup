#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

SemaphoreHandle_t xSemaphore; //Initialisierung Semaphor

void vTask1(void *pvParameters) {
    while (true) {
        int* p = (int*)pvParameters;
        if( xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE)
        {
            printf("Task 1 is running!\nÜbergebener Paramter: %d \n\n", *p);
            vTaskDelay(pdMS_TO_TICKS(500)); // Simulierte Arbeit für 500 ms
            xSemaphoreGive(xSemaphore);
            vTaskDelay(pdMS_TO_TICKS(2000));  // 1 Sekunde Pause
        }
    }
}

void vTask2(void *pvParameters) {
    while (true) {
        if( xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE){
        printf("Task 2 is running!\n\n");
        vTaskDelay(pdMS_TO_TICKS(500)); // Simulierte Arbeit für 500 ms
        xSemaphoreGive(xSemaphore);
        vTaskDelay(pdMS_TO_TICKS(2000));  // 2 Sekunden Pause
        }
    }
}

int main() {
    stdio_init_all();
    int a = 8; // Wird übergeben
    int* p_a = &a;
    xSemaphore = xSemaphoreCreateBinary(); // wird blockiert erstellt
    if (xSemaphore != NULL) 
        // Gib den Semaphore frei, damit er verwendet werden kann
        xSemaphoreGive(xSemaphore);
    // Erstelle Task 1
    xTaskCreate(vTask1, "Task 1", 256, p_a, 3, NULL);

    // Erstelle Task 2
    xTaskCreate(vTask2, "Task 2", 256, NULL, 1, NULL);

    // Starte den Scheduler
    vTaskStartScheduler();

    while (true) {
        // Sollte nie erreicht werden
    }
}
