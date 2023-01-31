#pragma once

#include <string>
#include "esp_event.h"
#include "nvs_flash.h"
#include "asio.hpp"


using asio::ip::tcp;

class session :public std::enable_shared_from_this<session>
{
public:
  session(tcp::socket socket);
        
  void start();
  void sendData(std::string data);
  tcp::socket& getSocket();

  
  void setInteractive(bool inc);
  bool getInteractive() const;

protected:
  void do_read(); 
  void do_write(std::size_t length);
  void sendResult(std::string res);

  tcp::socket socket_;
  enum { max_length = 1024 };
  char data_[max_length];
  bool isInteractive;
};