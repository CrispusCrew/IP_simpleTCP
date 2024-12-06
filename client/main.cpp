#include "client.h"
#include "PereodicEvent.hpp"

int main(int argc, char *argv[]) {
    try {
        uint_least16_t portTCP;
        std::string clientMsg;
        std::chrono::milliseconds pingPeriod;
        if (argc > 4) {
            portTCP = safeStringToUint16(argv[1]);
            clientMsg = std::string(argv[2]);
            pingPeriod = std::chrono::milliseconds(safeFloatToInt64(safeStrToFloat(argv[3]) * 1000.0));
        }
        else {
            scout << "Enter portTCP, clientMsg and pingPeriod" << std::endl;
            std::cin >> portTCP >> clientMsg; {std::string buf_; std::cin >> buf_; pingPeriod = std::chrono::milliseconds(safeFloatToInt64(safeStdStringToFloat(buf_) * 1000.0));}       
        }

        std::string serverIP = "127.0.0.1";
        std::shared_ptr<ClientFactory> client(new ClientFactory(serverIP, portTCP));

        PereodicEvent<ClientFactory, void (ClientFactory::*)(const std::string&), const std::string&>
        pereodicEvent(pingPeriod, client, &ClientFactory::sendMessage, std::cref(clientMsg));

        scout << "Type \"exit\" or \"e\" to stop client & leave" << std::endl;
        std::string inputLine;
        while (std::getline(std::cin, inputLine) && inputLine != "exit" && inputLine != "e");
    } 
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}