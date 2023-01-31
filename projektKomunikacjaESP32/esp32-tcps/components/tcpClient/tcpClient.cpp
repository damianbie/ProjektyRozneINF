#include "tcpClient.hpp"


tcpClient::tcpClient(asio::io_context& context, asio::ip::tcp::endpoint& endpoint)
    :_context(context), _socket(context){
    this->_socket.connect(endpoint);
}
tcpClient::~tcpClient()
{
    asio::post(this->_context, [this]() { this->_socket.close(); });
}
void tcpClient::connect(asio::ip::tcp::socket& _ep)
{
    
}
void tcpClient::write(std::string data)
{
    asio::async_write(this->_socket,
        asio::buffer(data.c_str(),
            data.length()),
        [this, data](std::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {
               std::cout<<"Wyslano dane => " << data << std::endl; 
            }
            else
            {
                this->_socket.close();
            }
        });
}
void tcpClient::do_read()
{
    std::string data;
    data.resize(100);
    char tab[100];
    std::cout<<"Czekanie"<<std::endl;
    asio::read(this->_socket,
        asio::buffer(tab, sizeof(char) * 100));
        // [this, data, tab](std::error_code ec, std::size_t l/*length*/) mutable
        // {
        //     tab[l] = 0;
        //     std::cout<<"Odebrano => "<< std::string(tab) << std::endl;   
        // }); 
        std::cout<<"Odebrano => "<< std::string(tab) << std::endl;
}