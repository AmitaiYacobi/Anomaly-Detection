/*
 * Server.h
 *
 *  Created on: Dec 13, 2020
 *      Author: Eli
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <fstream>
#include <iostream>

#include "CLI.h"
#include <pthread.h>
#include <thread>

using namespace std;

// edit your ClientHandler interface here:
class ClientHandler {
public:
    virtual void handle(int clientID) = 0;
};

// you can add helper classes here and implement on the cpp file

// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler : public ClientHandler {

    DefaultIO* dio;

public:
    virtual void handle(int clientID)
    {
        this->dio = new SocketIO(clientID);
        CLI cli(dio);
        cli.start();
    }
};

// implement on Server.cpp
class Server {
    thread* t; // the thread to run the start() method in
    string ip_adress;
    int port;
    int serverFD;
    struct sockaddr_in serverAddress;
    struct sockaddr_in clientAddress;
    bool stop_flag;

public:
    Server(int port) throw(const char*);
    virtual ~Server();
    /**
   * @brief Initializes a new socket.
   *
   */
    void initialize();

    /**
   * @brief Activates the server for listening and also activates given
   * ClientHandler for connecting to the new connected client.
   *
   * @param ch ClientHandler&
   */
    void start(ClientHandler& ch) throw(const char*);
    /**
   * @brief Closes the server.
   *
   */
    void stop();
};

#endif /* SERVER_H_ */
