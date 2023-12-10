#ifndef _SERVERMESSENGER_H_
#define _SERVERMESSENGER_H_

#include "../../common/BaseMessenger.h"
#include "./LinkedList.h"

class ServerMessenger : public BaseMessenger
{
private:
    const unsigned int port;
    int server_fd;
    LinkedList<int> *connectedClients;
    LinkedList<Message> *messages;
    MessageRouter router;

    void setupConnection() override
    {
        struct sockaddr_in address;
        int opt = 1;

        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd < 0)
        {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }

        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
        {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
        {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }

        if (listen(server_fd, 3) < 0)
        {
            perror("listen");
            exit(EXIT_FAILURE);
        }
    };

    void handleClient(int clientSocket)
    {
        connectedClients->add(clientSocket);

        while (running)
        {
            char buffer[1024] = {0};
            ssize_t valread = read(clientSocket, buffer, sizeof(buffer) - 1);
            if (valread <= 0)
            {
                if (valread < 0)
                    perror("read");
                break;
            }

            buffer[valread] = '\0';
            Message receivedMsg = Message::deserialize(std::string(buffer));
            processReceivedMessage(clientSocket, receivedMsg);
        }

        close(clientSocket);
        connectedClients->remove(clientSocket);
    }

public:
    ServerMessenger(unsigned int port)
        : BaseMessenger(), port(port), connectedClients(new LinkedList<int>()), messages(new LinkedList<Message>())
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

    virtual ~ServerMessenger()
    {
        delete connectedClients;
        delete messages;
    }

    void broadcastMessage(int senderId, const Message &message)
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

    int sendMessageToClient(const int &clientSocket, const Message &message)
    {
        std::cout << "Sending message: " << message << std::endl;
        return sendMessage(clientSocket, message);
    }

    int start() override
    {
        setupConnection();
        running = true;

        while (running)
        {
            struct sockaddr_in address;
            socklen_t addrlen = sizeof(address);

            int new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
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

    int stop() override
    {
        running = false;
        close(server_fd);
        return EXIT_SUCCESS;
    };

    void addMessage(const Message &message)
    {
        std::string content = message.content.substr(strlen(SERVER_BROADCAST_RESPONSE_PREFIX.c_str()));
        Message newMessage(message.user_id, MessageTarget::BROADCAST, message.timestamp, content);
        messages->add(newMessage);
    }

    std::vector<Message> getMessages(const std::string &message)
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
};

#endif
