#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "pico/cyw43_arch.h"
#include "lwip/sockets.h"
#include "pico/util/queue.h"
#include "lwip/apps/mqtt.h"
#include "mqtt_client.hpp"  // Falls du die Klasse ausgelagert hast
#include "mqtt_config.hpp"

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

// ============================================================
int main() {
    stdio_init_all();

    if (cyw43_arch_init()) {
        printf("cyw43_arch_init() failed\n");
        return 1;
    }

    cyw43_arch_enable_sta_mode();
    printf("Connecting to Wi‑Fi …\n");
    if (int result = cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD,
                                           CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("failed to connect Wi‑Fi.\n");
        printf("WLAN-Fehlercode: %d\n", result);
        return 1;
    }
    printf("Wi‑Fi connected.\n");

    // ---- MQTT nutzen ----
    MqttClient mqtt{};
    if (!mqtt.connect(MQTT_BROKER_ADDR, MQTT_BROKER_PORT)) {
        printf("MQTT connect failed\n");
        return 1;
    }

    mqtt.subscribe("test/topic");
    mqtt.publish("test/topic", "Hello from Pico W");


    // ---- Haupt‑Loop: Stack am Leben halten ----
    while (true) {
        cyw43_arch_poll();     // wichtig für Wi‑Fi & lwIP
        sleep_ms(100);
    }
}

