#include "server.h"

Session::Session(tcp::socket socket, Logger &log) : socket_(std::move(socket)), log(log) {}
Session::~Session() {}

void Session::start() {accepting();}
void Session::accepting() {
    auto selfShared(shared_from_this());
    socket_.async_read_some(asio::buffer(dataBuf_),
            [this, selfShared](std::error_code ec, std::size_t length) {
                if (!ec) {
                    if (!std::string(dataBuf_, length).empty()) {
                        scout << "Recieve msg: " << std::string(dataBuf_, length) << std::endl;
                        log.logString(std::string(dataBuf_, length));
                    }
                    else {
                        scout << "Recieve empty msg" << std::endl;
                    }
                    
                    accepting();
                }
                else if (ec == asio::error::eof) 
                    scout << "Connection closed by client." << std::endl;
                else 
                    scout << "Error during read: " << ec.message() << std::endl;
            });
    }

Server::Server(uint_least16_t portTCP, asio::io_context& io_context, const std::string pathToFile)
: acceptor_(io_context, tcp::endpoint(tcp::v4(), portTCP)), io_context_(io_context), log(pathToFile) {
    acceptingForSession();
}

Server::~Server() {}

void Server::acceptingForSession() {
    acceptor_.async_accept(
    [this](std::error_code ec, tcp::socket socket) {
        if (!ec) {
            tcp::endpoint remote_endpoint = socket.remote_endpoint();
            std::string client_ip = remote_endpoint.address().to_string();
            uint_least16_t client_port = remote_endpoint.port();

            scout << "Client connected from IP: " << client_ip << " Port: " << client_port << std::endl;
            
            std::thread([this](tcp::socket socketBuf) mutable {
                auto session = std::make_shared<Session>(std::move(socketBuf), log);
                session->start();
            }, std::move(socket)).detach();
        }
        else scout << "Error during accept: " << ec.message() << std::endl;
        acceptingForSession();
    });
}

ServerFactory::ServerFactory(uint_least16_t portTCP, const std::string pathToFile) : 
io_context_(), server_(portTCP, io_context_, pathToFile), mainThread_([this]() {io_context_.run();}) {
    scout << "Server has started on port: " << portTCP << std::endl;
}

ServerFactory::~ServerFactory() {
    io_context_.stop();
    if (mainThread_.joinable()) mainThread_.join(); 
}