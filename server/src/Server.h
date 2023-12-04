#ifndef _SERVER_H_
#define _SERVER_H_

#include "../../common/Message.h"
#include "./Database.h"
#include "./ServerMessenger.h"

void signalHandler(int signum);

class Server
{
private:
    Database *database;
    ServerMessenger *messenger;

public:
    Server() : Server(8080) {}
    Server(unsigned int port) : database(new Database()), messenger(new ServerMessenger(port)) {}
    ~Server()
    {
        delete database;
        delete messenger;
    }

    void handleEstablishConnectionRequest(const int &connectionId, const Message &msg)
    {
        printf("Received message: %s\n", msg.serialize().c_str());
        EstablishConnectionResponseMessage response(connectionId);
        messenger->sendMessageToClient(connectionId, response);

        return;
    }

    void handleDropConnectionRequest(const int &connectionId, const Message &msg)
    {
        printf("Received message: %s\n", msg.serialize().c_str());
        DropConnectionResponseMessage response(connectionId);
        messenger->sendMessageToClient(connectionId, response);
        return;
    }

    void handleRegisterRequest(const int &connectionId, const Message &msg)
    {
        printf("Received message: %s\n", msg.serialize().c_str());
        std::vector<std::string> parts;
        std::stringstream ss(msg.content);
        std::string part;

        while (std::getline(ss, part, ' '))
        {
            parts.push_back(part);
        }

        const size_t expectedParts = 3;
        if (parts.size() != expectedParts)
        {
            printf("Invalid serialized message format for %s\n", msg.serialize().c_str());
            RegisterResponseMessage response(connectionId, RegisterResponseCode::REGISTER_FAILED);
            messenger->sendMessageToClient(connectionId, response);
            return;
        }

        std::string username = parts[1];
        std::string password = parts[2];
        if (username.empty() || password.empty())
        {
            printf("Invalid username or password\n");
            RegisterResponseMessage response(connectionId, RegisterResponseCode::REGISTER_FAILED);
            messenger->sendMessageToClient(connectionId, response);
            return;
        }

        bool ok = this->database->add(username, password);
        if (!ok)
        {
            RegisterResponseMessage response(connectionId, RegisterResponseCode::REGISTER_FAILED);
            messenger->sendMessageToClient(connectionId, response);
            return;
        }

        RegisterResponseMessage response(connectionId, RegisterResponseCode::REGISTER_SUCCESS);
        messenger->sendMessageToClient(connectionId, response);
    }

    void handleLoginRequest(const int &connectionId, const Message &msg)
    {
        printf("Received message: %s\n", msg.serialize().c_str());

        std::vector<std::string> parts;
        std::stringstream ss(msg.content);
        std::string part;

        while (std::getline(ss, part, ' '))
        {
            parts.push_back(part);
        }

        const size_t expectedParts = 3;
        if (parts.size() != expectedParts)
        {
            printf("Invalid serialized message format for %s\n", msg.serialize().c_str());
            LoginResponseMessage response(connectionId, LoginResponseCode::LOGIN_FAILED);
            messenger->sendMessageToClient(connectionId, response);
            return;
        }

        std::string username = parts[1];
        std::string password = parts[2];
        if (username.empty() || password.empty())
        {
            printf("Invalid username or password\n");
            LoginResponseMessage response(connectionId, LoginResponseCode::LOGIN_FAILED);
            messenger->sendMessageToClient(connectionId, response);
            return;
        }
        bool ok = this->database->contains(username, password);
        if (!ok)
        {
            LoginResponseMessage response(connectionId, LoginResponseCode::LOGIN_FAILED);
            messenger->sendMessageToClient(connectionId, response);
            return;
        }

        LoginResponseMessage response(connectionId, LoginResponseCode::LOGIN_SUCCESS);
        messenger->sendMessageToClient(connectionId, response);
    }

    int start()
    {
        std::string establish_connection_response_handler_path = CLIENT_ESTABLISH_CONNECTION_REQUEST_PREFIX;
        auto establish_connection_response_handler = std::bind(&Server::handleEstablishConnectionRequest, this, std::placeholders::_1, std::placeholders::_2);
        messenger->registerHandler(establish_connection_response_handler_path, establish_connection_response_handler);

        std::string drop_connection_response_handler_path = CLIENT_DROP_CONNECTION_REQUEST_PREFIX;
        auto drop_connection_response_handler = std::bind(&Server::handleDropConnectionRequest, this, std::placeholders::_1, std::placeholders::_2);
        messenger->registerHandler(drop_connection_response_handler_path, drop_connection_response_handler);

        std::string register_response_handler_path = CLIENT_REGISTER_REQUEST_PREFIX;
        auto register_response_handler = std::bind(&Server::handleRegisterRequest, this, std::placeholders::_1, std::placeholders::_2);
        messenger->registerHandler(register_response_handler_path, register_response_handler);

        std::string login_response_handler_path = CLIENT_LOGIN_REQUEST_PREFIX;
        auto login_response_handler = std::bind(&Server::handleLoginRequest, this, std::placeholders::_1, std::placeholders::_2);
        messenger->registerHandler(login_response_handler_path, login_response_handler);

        return messenger->start();
    }

    int stop()
    {
        return messenger->stop();
    }
};

#endif // _SERVER_H_
