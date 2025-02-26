#pragma once

#include <memory>
#include <string>
#include <cstdint>
#include <thread>
#include <functional>

#include <asio.hpp>

#include "utilities.h"

class Client : public std::enable_shared_from_this<Client>{
private:
    asio::io_context io_context_;
    asio::ip::tcp::socket socket_;
    std::thread mainThread_;
    std::function <void()> event_;
    
    const std::string serverIP;
    const uint_least16_t portTCP;

public:
    Client (std::string serverIP, uint_least16_t portTCP);
    ~Client ();
    void connect();
    void sendMessage(const std::string& message);
};

class ClientFactory {
private:
    std::shared_ptr <Client> client;
public:
    ClientFactory (std::string serverIP, uint_least16_t portTCP);
    void connect();
    void sendMessage(const std::string& message);
};