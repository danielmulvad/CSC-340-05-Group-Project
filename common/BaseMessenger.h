#ifndef _BASEMESSENGER_H_
#define _BASEMESSENGER_H_

#include "./messages/Message.h"

using MessageHandler = std::function<void(const int &, const Message &)>;
using MessageRouter = std::unordered_map<std::string, MessageHandler>;

class BaseMessenger
{
protected:
    bool running;
    MessageRouter router;

    virtual void setupConnection() = 0;
    void processReceivedMessage(const int &connectionId, const Message &msg, const bool debug = true);

public:
    BaseMessenger();
    ~BaseMessenger();

    virtual int start() = 0;
    virtual int stop() = 0;

    void registerHandler(const std::string &path, MessageHandler handler);
};

#endif // _BASEMESSENGER_H_
