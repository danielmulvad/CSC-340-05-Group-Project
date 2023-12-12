#include "./BaseMessenger.h"
#include <iostream>

BaseMessenger::BaseMessenger() : running(false) {}
BaseMessenger::~BaseMessenger()
{
}

void BaseMessenger::processReceivedMessage(const int &connectionId, const Message &msg, const bool debug)
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

void BaseMessenger::registerHandler(const std::string &path, MessageHandler handler)
{
    router.emplace(path, std::move(handler));
}
