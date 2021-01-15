
#include "Server.h"

Server::Server(int port) throw(const char*)
{
    this->ip_adress = "127.0.0.1";
    this->port = port;
    this->stop_flag = false;
    serverFD = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFD < 0) {
        throw system_error(errno, generic_category(), "Cannot open socket");
    }
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    if (bind(serverFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        throw system_error(errno, generic_category(), "bind() call failed");
    }
    if (listen(serverFD, 5) < 0) {
        throw system_error(errno, generic_category(), "Error listening to new connections");
    }
}

/*void Server::initialize()
{

    serverFD = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFD < 0) {
        throw system_error(errno, generic_category(), "Cannot open socket");
    }
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    if (bind(serverFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        throw system_error(errno, generic_category(), "bind() call failed");
    }
    if (listen(serverFD, 5) < 0) {
        throw system_error(errno, generic_category(), "Error listening to new connections");
    }
}*/

void Server::start(ClientHandler& ch) throw(const char*)
{
    t = new thread([&ch, this]() {
        socklen_t clientAddressLength = sizeof(clientAddress);
        while (!stop_flag) {
            int clientID = accept(serverFD, (struct sockaddr*)&clientAddress, &clientAddressLength);
            if (clientID < 0) {
                throw "Error accepting new connection";
            }
            ch.handle(clientID);
            close(clientID);
        }
        close(serverFD);
    });
}

void Server::stop()
{
    stop_flag = true;
    t->join();
}

Server::~Server() { }
