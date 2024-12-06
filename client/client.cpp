#include "client.h"

Client::Client (std::string serverIP, uint_least16_t portTCP) :
serverIP(serverIP), portTCP(portTCP), 
io_context_(), mainThread_([&]() {io_context_.run();}), socket_(tcp::socket(io_context_)){}
  
Client::~Client() {
    io_context_.stop();
    if (mainThread_.joinable()) mainThread_.join(); 
}

void Client::connect() {
    asio::ip::tcp::resolver resolver(io_context_);
    asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(serverIP, std::to_string(portTCP));

    try {
        asio::connect(socket_, endpoints);

        if (socket_.is_open()) {
            scout << "Connected to: " << serverIP << ":" << portTCP << std::endl;
        } 
        else {
            scout << "Failed to connect: Socket not open" << std::endl;
        }
    }
    catch (const std::system_error& e) {
        scout << "Error during connect: " << e.what() << std::endl;
    }
}

void Client::sendMessage(const std::string& message) {
        try {
            asio::write(socket_, asio::buffer(message));
            scout << "Message sent: " << message << std::endl;
        } catch (const std::exception& e) {
            scout << "Failed to send message: " << e.what() << std::endl;
        }
    }  

ClientFactory::ClientFactory (std::string serverIP, uint_least16_t portTCP) {
    client = std::make_shared<Client>(serverIP, portTCP);
    client->connect();
}

void ClientFactory::connect() {
    client->connect();
}

void ClientFactory::sendMessage(const std::string& message) {
    client->sendMessage(message);
}
