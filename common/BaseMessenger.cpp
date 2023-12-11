#include "./BaseMessenger.h"

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

BaseMessenger::BaseMessenger() : running(false), socket_fd(-1) {}
BaseMessenger::~BaseMessenger()
{
    if (socket_fd > 0)
    {
        close(socket_fd);
    }
}

void BaseMessenger::registerHandler(const std::string &path, MessageHandler handler)
{
    router.emplace(path, std::move(handler));
}

int BaseMessenger::sendMessage(const int &clientSocket, const Message &msg)
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

int BaseMessenger::sendMessage(const Message &msg)
{
    return sendMessage(socket_fd, msg);
}
