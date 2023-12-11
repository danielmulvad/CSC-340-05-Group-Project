#ifndef _CLIENTMESSENGER_H_
#define _CLIENTMESSENGER_H_

#include "../../common/BaseMessenger.h"
#include <arpa/inet.h>

class ClientMessenger : public BaseMessenger
{
private:
    std::string serverAddress;
    unsigned int port;
    std::condition_variable exitConditionVariable;
    std::thread listenerThread;
    int socketId = 0;
    int connectionId = 0;

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