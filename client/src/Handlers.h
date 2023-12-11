#ifndef _HANDLERS_H_
#define _HANDLERS_H_

#include "./ClientMessenger.h"
#include "./Menu.h"
#include <thread>

class Handlers
{
private:
    ClientMessenger *messenger;
    std::thread messagingThread;

public:
    Handlers(ClientMessenger *messenger);
    ~Handlers();
    void broadcastHandler(const int &connectionId, const Message &msg);
    void dropConnectionHandler(const int &connectionId, const Message &msg);
    void establishConnectionHandler(const int &connectionId, const Message &msg);
    void handleLogin();
    void handleMainMenu();
    void handleRegister();
    void handleSearch(std::string &message);
    void handleStop();
    void loginHandler(const int &connectionId, const Message &msg);
    void registerHandler(const int &connectionId, const Message &msg);
    void searchHandler(const int &connectionId, const Message &msg);
    void startMessaging();
};

#endif // _HANDLERS_H_