#ifndef _REGISTER_H_
#define _REGISTER_H_

#include "./Message.h"

const static std::string CLIENT_REGISTER_REQUEST_PREFIX = "REGISTER_REQUEST";
const static std::string SERVER_REGISTER_RESPONSE_PREFIX = "REGISTER_RESPONSE";

enum RegisterResponseCode
{
    REGISTER_SUCCESS = 1,
    REGISTER_FAILED = 2
};

struct RegisterRequestMessage : public Message
{
    RegisterRequestMessage(const int &connection_id, const std::string &username, const std::string &password) : Message(connection_id, username, MessageTarget::CLIENT_TO_SERVER, CLIENT_REGISTER_REQUEST_PREFIX + " " + username + " " + password) {}
};

struct RegisterResponseMessage : public Message
{
    RegisterResponseMessage(const int &connection_id, const std::string &username, const RegisterResponseCode &response_code) : Message(connection_id, username, MessageTarget::SERVER_TO_CLIENT, SERVER_REGISTER_RESPONSE_PREFIX + " " + std::to_string(response_code)) {}
};

#endif // _REGISTER_H_