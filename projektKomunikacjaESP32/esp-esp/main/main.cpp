#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"

#include "WiFiConnection.hpp"
#include "server.hpp"
#include "System.hpp"

extern "C" 
{
   void app_main(void);
}

void app_main(void)
{
   System::getInstance().initFileSystem();
   WiFiConnection::instance().createNetwork("esp32Test", "ppp123abcAyA");
   
   asio::io_context io_context;
   server s(io_context, 3333);
   std::cout << "ASIO engine is up and running" << std::endl;
   xTaskCreate([](void* ptr) -> void {
      ((asio::io_context*)ptr)->run();
   }, "tcp_server", 4096, &io_context, 10, NULL);
   
   std::cout<<"Serwer jest uruchomiony"<<std::endl;
   while(true)
   {
      vTaskDelay(100); // just to keep the watchdog happy
   }
}