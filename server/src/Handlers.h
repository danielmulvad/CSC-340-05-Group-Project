#ifndef _HANDLERS_H_
#define _HANDLERS_H_

#include "../../common/messages/Broadcast.h"
#include "../../common/messages/DropConnection.h"
#include "../../common/messages/EstablishConnection.h"
#include "../../common/messages/Login.h"
#include "../../common/messages/Message.h"
#include "../../common/messages/Register.h"
#include "../../common/messages/Search.h"
#include "./database/Database.h"
#include "./ServerMessenger.h"

class Handlers
{
private:
    Database *database;
    ServerMessenger *messenger;

public:
    Handlers(Database *database, ServerMessenger *messenger);
    ~Handlers();

    void handleBroadcastRequest(const int &connectionId, const Message &msg);
    void handleDropConnectionRequest(const int &connectionId, const Message &msg);
    void handleEstablishConnectionRequest(const int &connectionId, const Message &msg);
    void handleLoginRequest(const int &connectionId, const Message &msg);
    void handleRegisterRequest(const int &connectionId, const Message &msg);
    void handleSearchRequest(const int &connectionId, const Message &msg);
};

#endif // _HANDLERS_H_