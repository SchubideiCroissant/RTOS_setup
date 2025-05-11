#include "mqtt_client.hpp"
#include "mqtt_config.hpp"
#include <string.h>
#include <stdio.h>
#include "lwip/ip_addr.h"


MqttClient::MqttClient() : client(mqtt_client_new()) {}

bool MqttClient::connect(const char *host_ip, u16_t port) {
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
    ci.keep_alive = 60;
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

bool MqttClient::publish(const char *topic, const char *payload,
                         int qos, bool retain) {
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

bool MqttClient::subscribe(const char *topic, int qos) {
    err_t err = mqtt_subscribe(client, topic, qos,
                               &MqttClient::s_sub_request_cb, this);
    if (err != ERR_OK) {
        printf("mqtt_subscribe returned: %d\n", err);
        return false;
    }
    return true;
}

void MqttClient::disconnect() {
    mqtt_disconnect(client);
}

// === Callbacks ===
void MqttClient::s_connection_cb(mqtt_client_t *c, void *arg,
                                 mqtt_connection_status_t status) {
    static_cast<MqttClient *>(arg)->connection_cb(c, status);
}
void MqttClient::s_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len) {
    static_cast<MqttClient *>(arg)->incoming_publish_cb(topic, tot_len);
}
void MqttClient::s_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags) {
    static_cast<MqttClient *>(arg)->incoming_data_cb(data, len, flags);
}
void MqttClient::s_sub_request_cb(void *arg, err_t result) {
    static_cast<MqttClient *>(arg)->sub_request_cb(result);
}

void MqttClient::connection_cb(mqtt_client_t *c, mqtt_connection_status_t status) {
    printf("MQTT connection status: %d\n", status);
    if (status == MQTT_CONNECT_ACCEPTED) {
        printf("MQTT connected.\n");
        mqtt_set_inpub_callback(c,
                                &MqttClient::s_incoming_publish_cb,
                                &MqttClient::s_incoming_data_cb,
                                this);
    }
}
void MqttClient::incoming_publish_cb(const char *topic, u32_t tot_len) {
    printf("[MQTT] Topic '%s', %lu bytes\n", topic, (unsigned long)tot_len);
}
void MqttClient::incoming_data_cb(const u8_t *data, u16_t len, u8_t flags) {
    printf("[MQTT] Payload: %.*s\n", len, data);
    if (flags & MQTT_DATA_FLAG_LAST) {
        printf("[MQTT] Ende der Nachricht\n");
    }
}
void MqttClient::sub_request_cb(err_t result) {
    printf("Subscribe result: %d\n", result);
}
