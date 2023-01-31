#include "session.hpp"

session::session(tcp::socket socket)
    :socket_(std::move(socket)), isInteractive(false)
{}
void session::start()
{
    // this->sendData("Hello! user!");
    this->do_read();
}
void session::do_read()
{
    auto self(shared_from_this());
    socket_.async_read_some(asio::buffer(data_, max_length),
        [this, self](std::error_code ec, std::size_t length)
        {
            if (!ec)
            {
                data_[length] = 0;
                do_write(length);
                std::cout<<" DATA => "<< data_<<std::endl;
            }
        });
}
void session::do_write(std::size_t length)
{
    auto self(shared_from_this());
    asio::async_write(socket_, asio::buffer(data_, length),
        [this, self](std::error_code ec, std::size_t length)
        {
            if (!ec)
            {
                do_read();
            }
        });
}
void session::sendData(std::string data)
{
    asio::const_buffer _buff(data.c_str(), data.size());
    asio::write(this->socket_, _buff);
}