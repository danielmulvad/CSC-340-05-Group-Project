#ifndef _SERVER_H_
#define _SERVER_H_

#include "./database/Database.h"
#include "./Handlers.h"
#include "./ServerMessenger.h"

void signalHandler(int signum);

class Server
{
private:
    Database *database;
    ServerMessenger *messenger;
    Handlers *handlers;

public:
    Server();
    Server(const unsigned int &port);
    ~Server();

    int start();
    int stop();
};

#endif // _SERVER_H_
