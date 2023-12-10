#ifndef _SERVER_H_
#define _SERVER_H_

#include "../../common/Message.h"
#include "./Database.h"
#include "./ServerMessenger.h"
#include "./Handlers.h"

void signalHandler(int signum);

class Server
{
private:
    Database *database;
    ServerMessenger *messenger;
    Handlers *handlers;

public:
    Server() : Server(8080) {}
    Server(unsigned int port) : database(new Database()), messenger(new ServerMessenger(port))
    {
        handlers = new Handlers(database, messenger);
    }
    ~Server()
    {
        delete database;
        delete messenger;
    }

    int start()
    {
        // Establish connection
        std::string establish_connection_response_handler_path = CLIENT_ESTABLISH_CONNECTION_REQUEST_PREFIX;
        auto establish_connection_response_handler = std::bind(&Handlers::handleEstablishConnectionRequest, handlers, std::placeholders::_1, std::placeholders::_2);
        messenger->registerHandler(establish_connection_response_handler_path, establish_connection_response_handler);

        // Drop connection
        std::string drop_connection_response_handler_path = CLIENT_DROP_CONNECTION_REQUEST_PREFIX;
        auto drop_connection_response_handler = std::bind(&Handlers::handleDropConnectionRequest, handlers, std::placeholders::_1, std::placeholders::_2);
        messenger->registerHandler(drop_connection_response_handler_path, drop_connection_response_handler);

        // Register
        std::string register_response_handler_path = CLIENT_REGISTER_REQUEST_PREFIX;
        auto register_response_handler = std::bind(&Handlers::handleRegisterRequest, handlers, std::placeholders::_1, std::placeholders::_2);
        messenger->registerHandler(register_response_handler_path, register_response_handler);

        // Login
        std::string login_response_handler_path = CLIENT_LOGIN_REQUEST_PREFIX;
        auto login_response_handler = std::bind(&Handlers::handleLoginRequest, handlers, std::placeholders::_1, std::placeholders::_2);
        messenger->registerHandler(login_response_handler_path, login_response_handler);

        // Broadcast
        std::string broadcast_handler_path = CLIENT_BROADCAST_REQUEST_PREFIX;
        auto broadcast_handler = std::bind(&Handlers::handleBroadcastRequest, handlers, std::placeholders::_1, std::placeholders::_2);
        messenger->registerHandler(broadcast_handler_path, broadcast_handler);

        // Search
        std::string search_handler_path = CLIENT_SEARCH_REQUEST_PREFIX;
        auto search_handler = std::bind(&Handlers::handleSearchRequest, handlers, std::placeholders::_1, std::placeholders::_2);
        messenger->registerHandler(search_handler_path, search_handler);

        return messenger->start();
    }

    int stop()
    {
        return messenger->stop();
    }
};

#endif // _SERVER_H_
