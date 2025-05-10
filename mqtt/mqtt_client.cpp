// mqtt_client_picow.cpp
// Minimal MQTT-Klasse auf Basis von lwIP/apps/mqtt.h für den Raspberry Pi Pico W
// ------------------------------------------------------------
// Voraussetzungen im pico-sdk:
//   - lwIP mit MQTT-Anteil aktiv (LWIP_ENABLE_MQTT = 1)
//   - cyw43_arch (Thread-safe-background)
//   - In CMakeLists.txt: pico_cyw43_arch_lwip_threadsafe_background und lwip(lwipcore+apps) einbinden
// ------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "lwip/apps/mqtt.h"
#include "lwip/ip_addr.h"
#include "pico/cyw43_arch.h"
#include "mqtt_client.hpp"
#include "mqtt_config.hpp"


// ============================================================
class MqttClient {
public:
    MqttClient() : client(mqtt_client_new()) {}

    bool connect(const char *host_ip, u16_t port) {
        if (!client) {
            printf("mqtt_client_new() failed\n");
            return false;
        }
        ip_addr_t broker_addr{};
        if (!ipaddr_aton(host_ip, &broker_addr)) {
            printf("ipaddr_aton() failed – prüfe Broker-Adresse\n");
            return false;
        }

        struct mqtt_connect_client_info_t ci{};
        ci.client_id  = MQTT_CLIENT_ID;
        ci.keep_alive = 60;          // Sekunden
        ci.client_user = MQTT_USER;
        ci.client_pass = MQTT_PASS;

        err_t err = mqtt_client_connect(client, &broker_addr, port,
                                        &MqttClient::s_connection_cb, this, &ci);
        if (err != ERR_OK) {
            printf("mqtt_client_connect error: %d\n", err);
            return false;
        }
        return true;
    }

    bool publish(const char *topic, const char *payload,
                 int qos = 0, bool retain = false) {
        err_t err = mqtt_publish(client, topic,
                                 payload, strlen(payload),
                                 qos, retain,
                                 NULL, NULL);
        if (err != ERR_OK) {
            printf("mqtt_publish returned: %d\n", err);
            return false;
        }
        return true;
    }

    bool subscribe(const char *topic, int qos = 0) {
        err_t err = mqtt_subscribe(client, topic, qos,
                                   &MqttClient::s_sub_request_cb, this);
        if (err != ERR_OK) {
            printf("mqtt_subscribe returned: %d\n", err);
            return false;
        }
        return true;
    }

    void disconnect() {
        mqtt_disconnect(client);
    }

private:
    mqtt_client_t *client;

    // ---- statische Callback‑Wrapper rufen Instanzfunktionen auf ----
    static void s_connection_cb(mqtt_client_t *c, void *arg,
                                mqtt_connection_status_t status) {
        static_cast<MqttClient *>(arg)->connection_cb(c, status);
    }
    static void s_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len) {
        static_cast<MqttClient *>(arg)->incoming_publish_cb(topic, tot_len);
    }
    static void s_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags) {
        static_cast<MqttClient *>(arg)->incoming_data_cb(data, len, flags);
    }
    static void s_sub_request_cb(void *arg, err_t result) {
        static_cast<MqttClient *>(arg)->sub_request_cb(result);
    }

    // ---- eigentliche Callback‑Implementierungen ----
    void connection_cb(mqtt_client_t *c, mqtt_connection_status_t status) {
        printf("MQTT connection status: %d\n", status);
        if (status == MQTT_CONNECT_ACCEPTED) {
            printf("MQTT connected.\n");
            mqtt_set_inpub_callback(c,
                                    &MqttClient::s_incoming_publish_cb,
                                    &MqttClient::s_incoming_data_cb,
                                    this);
        }
    }

    void incoming_publish_cb(const char *topic, u32_t tot_len) {
        printf("[MQTT] Topic '%s', %lu bytes\n", topic, (unsigned long)tot_len);
    }

    void incoming_data_cb(const u8_t *data, u16_t len, u8_t flags) {
        printf("[MQTT] Payload: %.*s\n", len, data);
        if (flags & MQTT_DATA_FLAG_LAST) {
            printf("[MQTT] Ende der Nachricht\n");
        }
    }

    void sub_request_cb(err_t result) {
        printf("Subscribe result: %d\n", result);
    }
};

