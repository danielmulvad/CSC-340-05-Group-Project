#ifndef _BROADCAST_H_
#define _BROADCAST_H_

#include "./Message.h"

// Broadcast
const static std::string CLIENT_BROADCAST_REQUEST_PREFIX = "BROADCAST_REQUEST";
const static std::string SERVER_BROADCAST_RESPONSE_PREFIX = "BROADCAST_RESPONSE";
struct BroadcastRequestMessage : public Message
{
    BroadcastRequestMessage(const int &connection_id, const std::string &username, const std::string &msg) : Message(connection_id, username, MessageTarget::CLIENT_TO_SERVER, CLIENT_BROADCAST_REQUEST_PREFIX + " " + msg) {}
};
struct BroadcastResponseMessage : public Message
{
    BroadcastResponseMessage(const int &connection_id, const std::string &username, const std::string &msg) : Message(connection_id, username, MessageTarget::BROADCAST, SERVER_BROADCAST_RESPONSE_PREFIX + " " + msg) {}
};

#endif // _BROADCAST_H_