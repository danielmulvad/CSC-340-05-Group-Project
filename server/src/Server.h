#ifndef _SERVER_H_
#define _SERVER_H_

#include "../../common/Message.h"
#include "./ServerMessenger.h"

void signalHandler(int signum);

class Server
{
private:
    ServerMessenger messenger;

public:
    Server() : Server(8080) {}
    Server(unsigned int port) : messenger(port) {}

    void handleEstablishConnectionRequest(const int &connectionId, const Message &msg)
    {
        printf("Received message: %s\n", msg.serialize().c_str());
        EstablishConnectionResponseMessage response(connectionId);
        messenger.sendMessageToClient(connectionId, response);
        return;
    }

    ~Server() {}

    int run()
    {
        std::string establish_connection_response_handler_path = CLIENT_TO_SERVER_PREFIX + " " + CLIENT_ESTABLISH_CONNECTION_REQUEST_PREFIX;
        messenger.registerHandler(establish_connection_response_handler_path, std::bind(&Server::handleEstablishConnectionRequest, this, std::placeholders::_1, std::placeholders::_2));
        messenger.start();
        return EXIT_SUCCESS;
    }
};

#endif // _SERVER_H_
