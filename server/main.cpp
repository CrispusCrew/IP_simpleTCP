#include "server.h"

int main(int argc, char *argv[]) {
    try {
        uint_least16_t portTCP;
        if (argc > 1) portTCP = safeStringToUint16(argv[1]);
        else {
            scout << "Enter portTCP" << std::endl;
            std::cin >> portTCP;       
        }
        ServerFactory server(portTCP);

        scout << "Type \"exit\" or \"e\" to stop server & leave" << std::endl;
        std::string inputLine;
        while (std::getline(std::cin, inputLine) && inputLine != "exit" && inputLine != "e");
    } 
    catch (std::exception& e) {
        scout << "Error: " << e.what() << std::endl;
    }
    return 0;
}
