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
#include <memory>

std::unique_ptr<MqttClient> mqtt;   // nur ein Zeiger ist global

SemaphoreHandle_t xSemaphore; //Initialisierung Semaphor

void cyw43_poll_task(void *pvParameters) {
    const TickType_t interval = pdMS_TO_TICKS(50); // 50 ms konstant
    TickType_t last_wake_time = xTaskGetTickCount();

    while (true) {
        cyw43_arch_poll(); // z. B. für MQTT/WLAN Verarbeitung
        vTaskDelayUntil(&last_wake_time, interval);
    }
}


void mqtt_alive_task(void *pvParameters) {
    const TickType_t interval = pdMS_TO_TICKS(1000);
    TickType_t last_wake_time = xTaskGetTickCount();

    const char* topic = "picow/status";
    const char* payload = "{\"status\":\"alive\"}";

    while (true) {
        mqtt->publish(topic, payload);
        vTaskDelayUntil(&last_wake_time, interval);
    }
}
void mqtt_receive_task(void *pvParameters) {
    const TickType_t interval = pdMS_TO_TICKS(1000); 
    TickType_t last_wake_time = xTaskGetTickCount();
    // Topic abonnieren
    mqtt->subscribe("picow/input", 0);

    while (true) {
        // Nachrichten kommen über Callbacks (du kannst hier z. B. blinkende LED etc. machen)
        vTaskDelay(pdMS_TO_TICKS(1000)); // oder blockiere auf Event/Queue
        vTaskDelayUntil(&last_wake_time, interval);
    }
}


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

    mqtt = std::make_unique<MqttClient>();          // jetzt ist LwIP initialisiert
    if (!mqtt->connect(MQTT_BROKER_ADDR, MQTT_BROKER_PORT)) {
        printf("MQTT connect failed\n");
        return 1;
    }
    else{
    
    printf("MQTT connect success\n");
    xTaskCreate(cyw43_poll_task, "MQTT_Poll", 1024, NULL, 2, NULL);
    xTaskCreate(mqtt_alive_task, "MQTT_Alive", 1024, NULL, 4, NULL);
    //xTaskCreate(mqtt_receive_task, "MQTT_RX", 1024, NULL, 1, NULL);
    vTaskStartScheduler();
    }

    while (true) {
       
    }
}

