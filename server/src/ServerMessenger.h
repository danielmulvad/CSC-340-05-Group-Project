#ifndef _SERVERMESSENGER_H_
#define _SERVERMESSENGER_H_

#include "../../common/BaseMessenger.h"
#include "../../common/Socket.h"
#include "../../common/messages/Message.h"
#include "./LinkedList.h"

class ServerMessenger : public BaseMessenger, public Socket
{
private:
    const unsigned int port;
    int server_fd;
    LinkedList<int> *connectedClients;
    LinkedList<Message> *messages;
    MessageRouter router;

    void handleClient(int clientSocket);
    void setupConnection() override;

public:
    ServerMessenger(unsigned int port);
    virtual ~ServerMessenger();

    int sendMessageToClient(const int &clientSocket, const Message &message);
    int start() override;
    int stop() override;
    std::vector<Message> getMessages(const std::string &message);
    void addMessage(const Message &message);
    void broadcastMessage(int senderId, const Message &message);
};

#endif // _SERVERMESSENGER_H_
