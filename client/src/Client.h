#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "../../common/Message.h"
#include "./ClientMessenger.h"
#include "./Handlers.h"
#include "./Menu.h"

class Client
{
private:
    ClientMessenger *messenger;
    Handlers *handlers;

    std::thread messagingThread;
    std::mutex exitMutex;
    std::condition_variable exitConditionVariable;

public:
    Client() : Client(8080) {}
    Client(unsigned int port) : messenger(new ClientMessenger("0.0.0.0", port))
    {
        handlers = new Handlers(messenger);
    }

    int start()
    {
        // Establish Connection
        std::string establish_connection_handler_pattern = SERVER_ESTABLISH_CONNECTION_RESPONSE_PREFIX;
        auto establish_connection_handler = std::bind(&Handlers::establishConnectionHandler, handlers, std::placeholders::_1, std::placeholders::_2);
        messenger->registerHandler(establish_connection_handler_pattern, establish_connection_handler);

        // Drop Connection
        std::string drop_connection_handler_pattern = SERVER_DROP_CONNECTION_RESPONSE_PREFIX;
        auto drop_connection_handler = std::bind(&Handlers::dropConnectionHandler, handlers, std::placeholders::_1, std::placeholders::_2);
        messenger->registerHandler(drop_connection_handler_pattern, drop_connection_handler);

        // Login
        std::string login_handler_pattern = SERVER_LOGIN_RESPONSE_PREFIX;
        auto login_handler = std::bind(&Handlers::loginHandler, handlers, std::placeholders::_1, std::placeholders::_2);
        messenger->registerHandler(login_handler_pattern, login_handler);

        // Register
        std::string register_handler_pattern = SERVER_REGISTER_RESPONSE_PREFIX;
        auto register_handler = std::bind(&Handlers::registerHandler, handlers, std::placeholders::_1, std::placeholders::_2);
        messenger->registerHandler(register_handler_pattern, register_handler);

        // Broadcast
        std::string broadcast_handler_pattern = SERVER_BROADCAST_RESPONSE_PREFIX;
        auto broadcast_handler = std::bind(&Handlers::broadcastHandler, handlers, std::placeholders::_1, std::placeholders::_2);
        messenger->registerHandler(broadcast_handler_pattern, broadcast_handler);

        return messenger->start();
    }

    int stop()
    {

        return messenger->stop();
    }

    ~Client()
    {
        delete messenger;
    }
};

#endif // _CLIENT_H_
