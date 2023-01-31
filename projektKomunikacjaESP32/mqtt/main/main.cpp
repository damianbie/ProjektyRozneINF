#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"

#include "WiFiConnection.hpp"
#include "System.hpp"

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_system.h"
#include "esp_event.h"

#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "mqtt_client.h"

#include <cstdlib>
#include <time.h>

extern "C" 
{
   void app_main(void);
}

static const char *TAG = "MQTT_EXAMPLE";


static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t)event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
}

void app_main(void)
{
    System::getInstance().initFileSystem();
    WiFiConnection::instance().connect("900/2100/2", "ppp123abcAyA");

    if(WiFiConnection::instance().isConnected()) 
        ESP_LOGE("SYSTEM", "Wifi error - ");

    esp_mqtt_client_config_t mqtt_cfg;
    memset(&mqtt_cfg, 0, sizeof(mqtt_cfg));
    mqtt_cfg.username = "admin";
    mqtt_cfg.password = "public";
    mqtt_cfg.port     = 1883;
    mqtt_cfg.host     = "192.168.1.239";  

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_start(client);
    esp_mqtt_client_register_event(client, esp_mqtt_event_id_t::MQTT_EVENT_ANY, mqtt_event_handler, NULL);
    vTaskDelay(1000);
 
    int msg_id;
    msg_id = esp_mqtt_client_subscribe(client, "/czujniki/smog", 0);
    ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

    msg_id = esp_mqtt_client_subscribe(client, "/czujniki/temperatura1", 1);
    ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

    srand(time(NULL));
    while(true)
    {
        char buff[100];
        itoa((rand() % 10) + 10, buff, 10);
        msg_id = esp_mqtt_client_publish(client, "/czujniki/temperatura2", buff, 0, 1, 0);
        ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
        vTaskDelay(5000); // just to keep the watchdog happy
    }
}