#ifndef _DROP_CONNECTION_H_
#define _DROP_CONNECTION_H_

#include "./Message.h"

const static std::string CLIENT_DROP_CONNECTION_REQUEST_PREFIX = "DROP_CONNECTION_REQUEST";
const static std::string SERVER_DROP_CONNECTION_RESPONSE_PREFIX = "DROP_CONNECTION_RESPONSE";

struct DropConnectionRequestMessage : public Message
{
    DropConnectionRequestMessage(const int &connection_id, const std::string &username) : Message(connection_id, username, MessageTarget::CLIENT_TO_SERVER, CLIENT_DROP_CONNECTION_REQUEST_PREFIX) {}
};
struct DropConnectionResponseMessage : public Message
{
    DropConnectionResponseMessage(const int &connection_id, const std::string &username) : Message(connection_id, username, MessageTarget::SERVER_TO_CLIENT, SERVER_DROP_CONNECTION_RESPONSE_PREFIX + " " + std::to_string(connection_id)) {}
};

#endif // _DROP_CONNECTION_H_