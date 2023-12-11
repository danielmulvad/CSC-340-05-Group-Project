#ifndef _ESTABLISH_CONNECTION_H_
#define _ESTABLISH_CONNECTION_H_

#include "./Message.h"

const static std::string CLIENT_ESTABLISH_CONNECTION_REQUEST_PREFIX = "ESTABLISH_CONNECTION_REQUEST";
const static std::string SERVER_ESTABLISH_CONNECTION_RESPONSE_PREFIX = "ESTABLISH_CONNECTION_RESPONSE";

struct EstablishConnectionRequestMessage : public Message
{
    EstablishConnectionRequestMessage(const int &connection_id) : Message(connection_id, MessageTarget::CLIENT_TO_SERVER, CLIENT_ESTABLISH_CONNECTION_REQUEST_PREFIX) {}
};

struct EstablishConnectionResponseMessage : public Message
{
    EstablishConnectionResponseMessage(const int &connection_id) : Message(connection_id, MessageTarget::SERVER_TO_CLIENT, SERVER_ESTABLISH_CONNECTION_RESPONSE_PREFIX + " " + std::to_string(connection_id)) {}
};

#endif // _ESTABLISH_CONNECTION_H_