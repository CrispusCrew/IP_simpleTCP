#include "client.h"

float safeStdStringToFloat(const std::string& str);
float safeStrToFloat(const char* str);
int64_t safeFloatToInt64(float flt);
uint_least16_t safeStringToUint16(const char* str);

int main(int argc, char *argv[]) {
    try {
        uint_least16_t portTCP;
        std::string clientName;
        std::chrono::milliseconds pingPeriod;
        if (argc > 4) {
            clientName = std::string(argv[1]);
            portTCP = safeStringToUint16(argv[2]);
            pingPeriod = std::chrono::milliseconds(safeFloatToInt64(safeStrToFloat(argv[3]) * 1000.0));
        }
        else {
            std::cout << "Enter clientName, portTCP and pingPeriod" << std::endl;
            std::cin >> clientName >> portTCP; {std::string buf_; std::cin >> buf_; pingPeriod = std::chrono::milliseconds(safeFloatToInt64(safeStdStringToFloat(buf_) * 1000.0));}       
        }

        std::string serverIP = "127.0.0.1";
        ClientFactory client(serverIP, portTCP, pingPeriod, clientName);

        std::cout << "Type \"exit\" or \"e\" to stop client & leave" << std::endl;
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
        uint_least16_t value = std::stoi(str);

        if (value < 0 || value > std::numeric_limits<uint_least16_t>::max()) {
            throw std::out_of_range("Out of range uint_least16_t");
        }

        return value;
    }
    catch (const std::invalid_argument&) {throw std::invalid_argument("Non number char * include");}
    catch (const std::out_of_range&) {throw std::out_of_range("Out of range uint_least16_t");}
}

float safeStdStringToFloat(const std::string& str) {
    try {
        return std::stof(str);
    } 
    catch (const std::invalid_argument& e) {throw std::invalid_argument("Non float char * include");}
    catch (const std::out_of_range& e) {throw std::out_of_range("Out of range float");}
}

int64_t safeFloatToInt64(float flt) {
    try {
        int64_t value = std::round(flt);

        if (flt < static_cast<float>(std::numeric_limits<int64_t>::min()) || 
            flt > static_cast<float>(std::numeric_limits<int64_t>::max())) {
            throw std::out_of_range("Out of range int64_t");
        }

        return value;
    }
    catch (const std::invalid_argument&) {throw std::invalid_argument("Non float input");}
    catch (const std::out_of_range&) {throw std::out_of_range("Out of range int64_t");}
}

float safeStrToFloat(const char* str) {
    return safeStdStringToFloat(std::string(str));
}

Client::Client (std::string serverIP, uint_least16_t portTCP, std::chrono::milliseconds eventPeriod, std::string clientName) :
serverIP(serverIP), portTCP(portTCP), eventPeriod(eventPeriod), clientName(clientName),
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

        std::cout << "Connect on: " << std::string(serverIP) << ":" << std::to_string(portTCP) << std::endl;

        if (socket_.is_open())
        PereodicEventLifeExtender = 
            std::shared_ptr<PereodicEvent<Client, void(Client::*)(const std::string&), const std::string&>>(
                new PereodicEvent<Client, void(Client::*)(const std::string&), const std::string&>(
                    eventPeriod,                        // Период
                    shared_from_this(),                 // Ссылка на объект
                    &Client::sendMessage,               // Указатель на метод
                    std::cref(clientName)               // Константная ссылка на clientName
                )
            );
    }
    catch (const std::system_error& e) {
        std::cerr << "Error during connect: " << e.what() << std::endl;
    }
}

void Client::sendMessage(const std::string& message) {
        try {
            asio::write(socket_, asio::buffer(message));
            std::cout << "Message sent: " << message << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Failed to send message: " << e.what() << std::endl;
        }
    }

ClientFactory::ClientFactory (std::string serverIP, uint_least16_t portTCP, std::chrono::milliseconds eventPeriod, std::string clientName) {
    client = std::make_shared<Client>(serverIP, portTCP, eventPeriod, clientName);
    client->connect();
}

void ClientFactory::connect() {
    client->connect();
}

void ClientFactory::sendMessage(const std::string& message) {
    client->sendMessage(message);
}
