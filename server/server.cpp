#include "server.h"

uint_least16_t safeStringToUint16(const char* str);


int main(int argc, char *argv[]) {
    try {
        uint_least16_t portTCP;
        if (argc > 1) portTCP = safeStringToUint16(argv[1]);
        else {
            std::cout << "Enter portTCP" << std::endl;
            std::cin >> portTCP;       
        }
        ServerFactory server(portTCP);

        std::cout << "Type \"exit\" or \"e\" to stop server & leave" << std::endl;
        std::string inputLine;
        while (std::getline(std::cin, inputLine) && inputLine != "exit" && inputLine != "e") {
            ;
        }
    } 
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}

uint_least16_t safeStringToUint16(const char* str) {
    try {
        int value = std::stoi(str);

        if (value < 0 || value > std::numeric_limits<uint_least16_t>::max()) {
            throw std::out_of_range("Out of range uint_least16_t");
        }

        return static_cast<uint_least16_t>(value);
    }
    catch (const std::invalid_argument&) {
        throw std::invalid_argument("Non number char include");
    }
    catch (const std::out_of_range&) {
        throw std::out_of_range("Out of range uint_least16_t");
    }
}


Session::Session(tcp::socket socket, Logger &log) : socket_(std::move(socket)), log(log) {}
Session::~Session() {}

void Session::start() {accepting();}
void Session::accepting() {
    auto selfShared(shared_from_this());
    socket_.async_read_some(asio::buffer(dataBuf_),
            [this, selfShared](std::error_code ec, std::size_t length) {
                if (!ec) {
                    std::cout << "Recieve msg: " << std::string(dataBuf_, length) << std::endl;
                    log.logString(std::string(dataBuf_, length));
                    accepting();
                }
                else if (ec == asio::error::eof) 
                    std::cout << "Connection closed by client." << std::endl;
                else 
                    std::cerr << "Error during read: " << ec.message() << std::endl;
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

            std::cout << "Client connected from IP: " << client_ip << " Port: " << client_port << std::endl;
            
            std::thread([this](tcp::socket socketBuf) mutable {
                auto session = std::make_shared<Session>(std::move(socketBuf), log);
                session->start();
            }, std::move(socket)).detach();
        }
        else std::cerr << "Error during accept: " << ec.message() << std::endl;
        acceptingForSession();
    });
}

ServerFactory::ServerFactory(uint_least16_t portTCP, const std::string pathToFile) : 
io_context_(), server_(portTCP, io_context_, pathToFile), mainThread_([this]() {io_context_.run();}) {
    std::cout << "Server has started on port: " << portTCP << std::endl;
}

ServerFactory::~ServerFactory() {
    io_context_.stop();
    if (mainThread_.joinable()) mainThread_.join(); 
}