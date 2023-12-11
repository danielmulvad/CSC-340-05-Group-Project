#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "./ClientMessenger.h"
#include "./Handlers.h"

class Client
{
private:
    ClientMessenger *messenger;
    Handlers *handlers;

    std::thread messagingThread;
    std::mutex exitMutex;
    std::condition_variable exitConditionVariable;

public:
    Client();
    Client(unsigned int port);
    ~Client();

    int start();
    int stop();
};

#endif // _CLIENT_H_
