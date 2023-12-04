#ifndef _SERVER_H_
#define _SERVER_H_

#include "../../common/Message.h"
#include "./ServerMessenger.h"

void signalHandler(int signum);

class Server
{
private:
    ServerMessenger messenger;

public:
    Server() : Server(8080) {}
    Server(unsigned int port) : messenger(port) {}

    void handleEstablishConnectionRequest(const int &connectionId, const Message &msg)
    {
        printf("Received message: %s\n", msg.serialize().c_str());
        EstablishConnectionResponseMessage response(connectionId);
        messenger.sendMessageToClient(connectionId, response);

        // TODO: Add connection to Messenger LinkedList<int> *connectedClients;

        return;
    }

    void handleDropConnectionRequest(const int &connectionId, const Message &msg)
    {
        printf("Received message: %s\n", msg.serialize().c_str());
        DropConnectionResponseMessage response(connectionId);
        messenger.sendMessageToClient(connectionId, response);

        // TODO: Remove connection from Messenger LinkedList<int> *connectedClients;

        return;
    }

    void handleRegisterRequest(const int &connectionId, const Message &msg)
    {
        printf("Received message: %s\n", msg.serialize().c_str());
        std::string username, password;
        std::istringstream iss(msg.content);
        iss >> username >> password;
        if (username.empty() || password.empty())
        {
            printf("Invalid username or password\n");
            return;
        }

        // TODO: Save username/password to file

        RegisterResponseMessage response(connectionId, RegisterResponseCode::REGISTER_SUCCESS);
        messenger.sendMessageToClient(connectionId, response);
        return;
    }

    void handleLoginRequest(const int &connectionId, const Message &msg)
    {
        printf("Received message: %s\n", msg.serialize().c_str());
        std::string username, password;
        std::istringstream iss(msg.content);
        iss >> username >> password;
        if (username.empty() || password.empty())
        {
            printf("Invalid username or password\n");
            return;
        }

        // TODO: Check username/password from file

        LoginResponseMessage response(connectionId, LoginResponseCode::LOGIN_SUCCESS);
        messenger.sendMessageToClient(connectionId, response);
        return;
    }

    ~Server() {}

    int start()
    {
        std::string establish_connection_response_handler_path = CLIENT_TO_SERVER_PREFIX + " " + CLIENT_ESTABLISH_CONNECTION_REQUEST_PREFIX;
        auto establish_connection_response_handler = std::bind(&Server::handleEstablishConnectionRequest, this, std::placeholders::_1, std::placeholders::_2);
        messenger.registerHandler(establish_connection_response_handler_path, establish_connection_response_handler);

        std::string drop_connection_response_handler_path = CLIENT_TO_SERVER_PREFIX + " " + CLIENT_DROP_CONNECTION_REQUEST_PREFIX;
        auto drop_connection_response_handler = std::bind(&Server::handleDropConnectionRequest, this, std::placeholders::_1, std::placeholders::_2);
        messenger.registerHandler(drop_connection_response_handler_path, drop_connection_response_handler);

        std::string register_response_handler_path = CLIENT_TO_SERVER_PREFIX + " " + CLIENT_REGISTER_REQUEST_PREFIX;
        auto register_response_handler = std::bind(&Server::handleRegisterRequest, this, std::placeholders::_1, std::placeholders::_2);
        messenger.registerHandler(register_response_handler_path, register_response_handler);

        std::string login_response_handler_path = CLIENT_TO_SERVER_PREFIX + " " + CLIENT_LOGIN_REQUEST_PREFIX;
        auto login_response_handler = std::bind(&Server::handleLoginRequest, this, std::placeholders::_1, std::placeholders::_2);
        messenger.registerHandler(login_response_handler_path, login_response_handler);

        return messenger.start();
    }

    int stop()
    {
        return messenger.stop();
    }
};

#endif // _SERVER_H_
