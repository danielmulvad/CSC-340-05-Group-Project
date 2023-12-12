#include "../../common/messages/Broadcast.h"
#include "./ServerMessenger.h"
#include <arpa/inet.h>
#include <iostream>
#include <thread>

ServerMessenger::ServerMessenger(const unsigned int &port)
    : BaseMessenger(), connectedClients(new LinkedList<int>()), messages(new LinkedList<Message>()), socket(new Socket()), port(port), server_fd(-1)
{
    if (port > 65535)
    {
        throw std::invalid_argument("Port must be less than 65535");
    }
    else if (port < 1024)
    {
        throw std::invalid_argument("Port must be greater than 1024");
    }
}

ServerMessenger::~ServerMessenger()
{
    delete connectedClients;
    delete messages;
    delete socket;
}

void ServerMessenger::setupConnection()
{
    struct sockaddr_in address;
    int opt = 1;

    server_fd = socket->createSocket(AF_INET, SOCK_STREAM, 0);

    socket->setSocketOptions(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    socket->bindSocket(server_fd, (struct sockaddr *)&address, sizeof(address));

    socket->listenSocket(server_fd, 3);
}

void ServerMessenger::handleClient(int clientSocket)
{
    connectedClients->add(clientSocket);

    while (running)
    {
        try
        {
            char buffer[1024] = {0};
            ssize_t valread = socket->readSocket(clientSocket, buffer, sizeof(buffer) - 1);
            if (valread <= 0)
            {
                break; // Client disconnected
            }

            buffer[valread] = '\0';
            Message receivedMsg = Message::deserialize(std::string(buffer));
            processReceivedMessage(clientSocket, receivedMsg);
        }
        catch (std::exception &e)
        {
            std::cout << "Error in handleClient: " << e.what() << std::endl;
        }
    }

    socket->closeSocket(clientSocket);
    connectedClients->remove(clientSocket);
}

void ServerMessenger::broadcastMessage(int senderId, const Message &message)
{
    Node<int> *current = connectedClients->getHead();
    while (current != nullptr)
    {
        if (current->getValue() != senderId)
        {
            sendMessageToClient(current->getValue(), message);
        }
        current = current->getNext();
    }
}

int ServerMessenger::sendMessageToClient(const int &clientSocket, const Message &message)
{
    std::cout << "Sending message: " << message << std::endl;
    return socket->sendSocket(clientSocket, message.serialize().c_str(), message.serialize().length(), 0);
}

int ServerMessenger::start()
{
    setupConnection();
    running = true;

    while (running)
    {
        struct sockaddr_in address;
        socklen_t addrlen = sizeof(address);

        int new_socket = socket->acceptSocket(server_fd, (struct sockaddr *)&address, &addrlen);
        if (new_socket < 0)
        {
            perror("accept");
            continue;
        }

        std::thread clientThread(&ServerMessenger::handleClient, this, new_socket);
        clientThread.detach();
    }

    return EXIT_SUCCESS;
}

int ServerMessenger::stop()
{
    running = false;
    socket->closeSocket(server_fd);
    return EXIT_SUCCESS;
};

void ServerMessenger::addMessage(const Message &message)
{
    std::string content = message.content.substr(strlen(SERVER_BROADCAST_RESPONSE_PREFIX.c_str()));
    Message newMessage(message.connectionId, message.username, MessageTarget::BROADCAST, message.timestamp, content);
    messages->add(newMessage);
}

std::vector<Message> ServerMessenger::getMessages(const std::string &message)
{
    std::vector<Message> result;
    Node<Message> *current = messages->getHead();
    while (current != nullptr)
    {
        if (current->getValue().content.find(message) != std::string::npos)
        {
            result.push_back(current->getValue());
        }
        current = current->getNext();
    }
    return result;
}
