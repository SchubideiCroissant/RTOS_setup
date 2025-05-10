#ifndef MQTT_CLIENT_HPP
#define MQTT_CLIENT_HPP

#include "lwip/apps/mqtt.h"
#include "lwip/ip_addr.h"


class MqttClient {
public:
    MqttClient();
    bool connect(const char *host_ip, u16_t port);
    bool publish(const char *topic, const char *payload, int qos = 0, bool retain = false);
    bool subscribe(const char *topic, int qos = 0);
    void disconnect();

private:
    mqtt_client_t *client;

    static void s_connection_cb(mqtt_client_t *c, void *arg, mqtt_connection_status_t status);
    static void s_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len);
    static void s_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags);
    static void s_sub_request_cb(void *arg, err_t result);

    void connection_cb(mqtt_client_t *c, mqtt_connection_status_t status);
    void incoming_publish_cb(const char *topic, u32_t tot_len);
    void incoming_data_cb(const u8_t *data, u16_t len, u8_t flags);
    void sub_request_cb(err_t result);
};

#endif // MQTT_CLIENT_HPP
