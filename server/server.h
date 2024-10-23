#include "include.h"

using asio::ip::tcp;

#include "logger.h"

class Session : public std::enable_shared_from_this<Session> {
private:
    tcp::socket socket_;
    char dataBuf_[1024];
    void accepting();
    Logger &log;
public:
    Session(tcp::socket socket, Logger &log);
    ~Session();

    void start();
};

class Server {
private:
    tcp::acceptor acceptor_;
    asio::io_context& io_context_;
    void acceptingForSession();
    Logger log;
public:  
    Server(uint_least16_t portTCP, asio::io_context& io_context, const std::string pathToFile = standartLogPathToFile);
    ~Server();
};

class ServerFactory {
private:
    asio::io_context io_context_;
    Server server_;
    std::thread mainThread_;
public:
    ServerFactory(uint_least16_t portTCP, const std::string pathToFile = standartLogPathToFile);
    ~ServerFactory();
};