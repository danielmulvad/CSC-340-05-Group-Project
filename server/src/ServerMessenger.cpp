#include "../../common/messages/Broadcast.h"
#include "./ServerMessenger.h"

void ServerMessenger::setupConnection()
{
    struct sockaddr_in address;
    int opt = 1;

    server_fd = createSocket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setSocketOptions(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bindSocket(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listenSocket(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
}

void ServerMessenger::handleClient(int clientSocket)
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

ServerMessenger::ServerMessenger(unsigned int port)
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

ServerMessenger::~ServerMessenger()
{
    delete connectedClients;
    delete messages;
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
    return sendMessage(clientSocket, message);
}

int ServerMessenger::start()
{
    setupConnection();
    running = true;

    while (running)
    {
        struct sockaddr_in address;
        socklen_t addrlen = sizeof(address);

        int new_socket = acceptSocket(server_fd, (struct sockaddr *)&address, &addrlen);
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
    closeSocket(server_fd);
    return EXIT_SUCCESS;
};

void ServerMessenger::addMessage(const Message &message)
{
    std::string content = message.content.substr(strlen(SERVER_BROADCAST_RESPONSE_PREFIX.c_str()));
    Message newMessage(message.user_id, MessageTarget::BROADCAST, message.timestamp, content);
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
