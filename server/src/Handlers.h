#ifndef _HANDLERS_H_
#define _HANDLERS_H_
#include "../../common/Message.h"
#include "../../common/BaseMessenger.h"
#include "./Database.h"

class Handlers
{
private:
    Database *database;
    ServerMessenger *messenger;

public:
    Handlers(Database *database, ServerMessenger *messenger) : database(database), messenger(messenger) {}

    void handleEstablishConnectionRequest(const int &connectionId, const Message &msg)
    {
        EstablishConnectionResponseMessage response(connectionId);
        messenger->sendMessageToClient(connectionId, response);
        return;
    }

    void handleDropConnectionRequest(const int &connectionId, const Message &msg)
    {
        DropConnectionResponseMessage response(connectionId);
        messenger->sendMessageToClient(connectionId, response);
        return;
    }

    void handleRegisterRequest(const int &connectionId, const Message &msg)
    {
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

    void handleBroadcastRequest(const int &connectionId, const Message &msg)
    {
        std::string content = msg.content;
        content.erase(0, strlen(CLIENT_BROADCAST_REQUEST_PREFIX.c_str()) + 1);

        BroadcastResponseMessage broadcastResponse(connectionId, content);
        messenger->broadcastMessage(connectionId, broadcastResponse);
    }
};

#endif