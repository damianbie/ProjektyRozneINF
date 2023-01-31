#pragma once
#include "asio.hpp"
#include "session.hpp"

class server
{
public:
    server(asio::io_context& io_context, short port);

protected:
  void do_accept();

  tcp::acceptor acceptor_;
};