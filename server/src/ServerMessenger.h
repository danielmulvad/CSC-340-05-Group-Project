#ifndef _SERVERMESSENGER_H_
#define _SERVERMESSENGER_H_

#include "../../common/BaseMessenger.h"
#include "../../common/messages/Message.h"
#include "./LinkedList.h"

class ServerMessenger : public BaseMessenger
{
private:
    const unsigned int port;
    int server_fd;
    LinkedList<int> *connectedClients;
    LinkedList<Message> *messages;
    MessageRouter router;

    void handleClient(int clientSocket);
    void setupConnection() override;

protected:
    virtual int createSocket(int domain, int type, int protocol)
    {
        return socket(domain, type, protocol);
    }

    virtual int setSocketOptions(int sockfd, int level, int optname, const void *optval, socklen_t optlen)
    {
        return setsockopt(sockfd, level, optname, optval, optlen);
    }

    virtual int bindSocket(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
    {
        return bind(sockfd, addr, addrlen);
    }

    virtual int listenSocket(int sockfd, int backlog)
    {
        return listen(sockfd, backlog);
    }

    virtual int acceptSocket(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
    {
        return accept(sockfd, addr, addrlen);
    }

    virtual ssize_t readSocket(int sockfd, void *buf, size_t count)
    {
        return read(sockfd, buf, count);
    }

    virtual int closeSocket(int sockfd)
    {
        return close(sockfd);
    }

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
