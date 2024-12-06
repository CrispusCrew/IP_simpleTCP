# Simple TCP Server-Client

## Description
`Simple TCP Server-Client` is a basic C++ application implementing a client-server model using the **Asio** library for asynchronous input-output operations. The server can accept connections from clients, handle incoming messages, and log them. The client supports periodic message sending to the server at a defined interval. This project is being developed for self-educational purposes, but it can be used for any of your tasks. In particular, you can build something more complex using the example provided in the existing main.cpp.

## Features
### Server:
- Asynchronous acceptance of client connections.
- Logging of incoming messages.
- Multithreaded handling of each connection.

### Client:
- Connect to the server by IP and port.
- Periodic message sending with a specified interval.
- Automatic retrying of message sending after successful connection.

## Requirements
- Unix-type system
- CMake 3.18+
- C++11 compatible compiler
- [Asio](https://think-async.com/Asio/) library (included in the project)

## Building the Project

1. Clone the repository:

    ```bash
    $ git clone https://github.com/CrispusCrew/IP_simpleTCP.git
    $ cd IP_simpleTCP
2. Build the project with CMake & install the binaries:

    ```bash
    $ cmake -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" -B build

    $ cmake --build build --target client server

    $ cmake --install build
## Usage

### Server

- Start the server, specifying the port to listen on by args:

    ```bash
    $ ./bin/server <port>
- Or use cmd input after start

    ```bash
    $ ./bin/server
    Enter portTCP
    <port>
- Stop server by typing "exit" or simple "e"

    ```bash
    exit
### Client

- Start the client, specifying the client name, port and ping period on by args:

    ```bash
    $ ./bin/client <portTCP> <clientMsg> <pingPeriod>
- Or use cmd input after start

    ```bash
    $ ./bin/client
    Enter clientMsg, portTCP and pingPeriod
    <portTCP> <clientMsg> <pingPeriod>
- You also can use float value of pingPeriod (for example every 0.5 seconds)

    ```bash
    $ ./bin/client name1 12345 0.5
- Stop client by typing "exit" or simple "e"

    ```bash
    exit 
