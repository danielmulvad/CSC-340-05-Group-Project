#ifndef _CLIENTMESSENGER_H_
#define _CLIENTMESSENGER_H_

#include "../../common/BaseMessenger.h"
#include "../../common/Socket.h"
#include <arpa/inet.h>
#include <thread>

class ClientMessenger : public BaseMessenger, public Socket
{
private:
    std::string serverAddress;
    unsigned int port;
    std::condition_variable exitConditionVariable;
    std::thread listenerThread;
    int socket_fd = -1;
    int connectionId = -1;

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