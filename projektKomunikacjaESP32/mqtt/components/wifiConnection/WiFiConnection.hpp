#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */

#ifndef WIFI_CONNECTION_H
#define WIFI_CONNECTION_H

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

//Singleton
class WiFiConnection
{
    public:
    static WiFiConnection& instance();
    void connect(const char*, const char*);
    bool isConnected();


    const int WIFI_CONNECTED_BIT2 = BIT0;
    private:
    WiFiConnection();

    static void event_handler(void* arg, esp_event_base_t event_base,
                int32_t event_id, void* event_data);

    static void wifi_disconect_event(void* arg, esp_event_base_t event_base,
                int32_t event_id, void* event_data);
    
    /* FreeRTOS event group to signal when we are connected*/
    EventGroupHandle_t s_wifi_event_group;
    const char *TAG;
    bool _isConnected;

    esp_event_handler_instance_t instance_wifi_disconnected;
};
#endif