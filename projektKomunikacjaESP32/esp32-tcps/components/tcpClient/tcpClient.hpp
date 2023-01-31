#include <asio.hpp>
#include <string>
#include <iostream>

class tcpClient
{
    public:
    tcpClient(asio::io_context& context, asio::ip::tcp::endpoint& endpoint);
    ~tcpClient();

    void write(std::string data);
    void do_read();

    private: 
    asio::io_context& _context;
    // asio::ip::tcp::resolver::results_type _socket;
    asio::ip::tcp::socket _socket;

    void connect(asio::ip::tcp::socket& _endpoint);
};
