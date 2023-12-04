#ifndef _BASEMESSENGER_H_
#define _BASEMESSENGER_H_

#include "./Message.h"
#include <iostream>
#include <netinet/in.h>
#include <thread>
#include <unistd.h>

using MessageHandler = std::function<void(const int &, const Message &)>;
using MessageRouter = std::unordered_map<std::string, MessageHandler>;

class BaseMessenger
{
protected:
    bool running;
    int socket_fd;
    MessageRouter router;

    virtual void setupConnection() = 0;
    void processReceivedMessage(const int &connectionId, const Message &msg, const bool debug = true)
    {
        for (const auto &entry : router)
        {
            if (msg.content.find(entry.first) != std::string::npos)
            {
                if (debug)
                {
                    std::cout << "Received a message: " << msg.toString() << std::endl;
                }
                entry.second(connectionId, msg);
                return;
            }
        }
        std::cout << "Received an unhandled message: " << msg.toString() << std::endl;
    };

public:
    BaseMessenger() : running(false), socket_fd(-1) {}
    ~BaseMessenger()
    {
        if (socket_fd > 0)
        {
            close(socket_fd);
        }
    }

    virtual int start() = 0;
    virtual int stop() = 0;

    void registerHandler(const std::string &path, MessageHandler handler)
    {
        router.emplace(path, std::move(handler));
    }

    int sendMessage(const int &clientSocket, const Message &msg)
    {
        if (clientSocket <= 0)
        {
            std::cerr << "Socket is not connected." << std::endl;
            return EXIT_FAILURE;
        }
        std::string serializedMsg = msg.serialize();
        ssize_t result = send(clientSocket, serializedMsg.c_str(), serializedMsg.length(), 0);
        if (result < 0)
        {
            std::cerr << "Failed to send message." << std::endl;
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }

    int sendMessage(const Message &msg)
    {
        return sendMessage(socket_fd, msg);
    }
};

#endif
