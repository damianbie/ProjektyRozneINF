#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_now.h"

#include "WiFiConnection.hpp"
#include "tcpClient.hpp"
#include "System.hpp"
#include <string>
#include <time.h>
#include <iostream>

extern "C" 
{
   void app_main(void);
}

void app_main(void)
{
   System::getInstance().initFileSystem();
   WiFiConnection::instance().connect("900/2100/2", "ppp123abcAyA");

   if(WiFiConnection::instance().isConnected()) 
      ESP_LOGE("SYSTEM", "Wifi error - ");


   asio::io_context io_context;
   asio::ip::tcp::endpoint serverAddress(asio::ip::address_v4::from_string("192.168.1.239"), 3333);
   tcpClient client(io_context, serverAddress);
   
   // idle task
   asio::executor_work_guard<decltype(io_context.get_executor())> work{io_context.get_executor()};
   std::cout << "ASIO engine is up and running" << std::endl;
   xTaskCreate([](void* ptr) -> void {
      ((asio::io_context*)ptr)->run();
   }, "tcp_server", 4096, &io_context, 10, NULL);
   srand(time(NULL));
   while(true)
   {  
      char buff[100];
      itoa(rand()%20 + 10, buff, 10);
      client.write(std::string(buff));
      vTaskDelay(1000);
   }

}