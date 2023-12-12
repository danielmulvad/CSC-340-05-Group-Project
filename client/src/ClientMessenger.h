#ifndef _CLIENTMESSENGER_H_
#define _CLIENTMESSENGER_H_

#include "../../common/BaseMessenger.h"
#include "../../common/Socket.h"
#include <arpa/inet.h>
#include <thread>

class ClientMessenger : public BaseMessenger
{
private:
    Socket *socket;

    int connectionId;
    int socket_fd;
    std::condition_variable exitConditionVariable;
    std::string serverAddress;
    std::thread listenerThread;
    unsigned int port;

    void setupConnection() override;
    void listenForMessages();

public:
    ClientMessenger(const std::string &serverAddress, unsigned int port);
    virtual ~ClientMessenger();

    int getConnectionId();
    void setConnectionId(const int &connectionId);

    int start() override;
    int stop() override;

    void registerHandler(const std::string &pattern, MessageHandler handler);

    void sendMessageToServer(const Message &msg);
};

#endif // _CLIENTMESSENGER_H_