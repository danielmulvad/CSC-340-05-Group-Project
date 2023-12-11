#ifndef _SEARCH_H_
#define _SEARCH_H_

#include "./Message.h"

const static std::string CLIENT_SEARCH_REQUEST_PREFIX = "SEARCH_REQUEST";
const static std::string SERVER_SEARCH_RESPONSE_PREFIX = "SEARCH_RESPONSE";

struct SearchRequestMessage : public Message
{
    SearchRequestMessage(const int &connection_id, const std::string &msg) : Message(connection_id, MessageTarget::CLIENT_TO_SERVER, CLIENT_SEARCH_REQUEST_PREFIX + " " + msg) {}
};

struct SearchResponseMessage : public Message
{
    SearchResponseMessage(const Message &message) : Message(message.user_id, MessageTarget::SERVER_TO_CLIENT, message.timestamp, SERVER_SEARCH_RESPONSE_PREFIX + " " + message.content) {}
};

#endif // _SEARCH_H_