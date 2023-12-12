#ifndef _LOGIN_H_
#define _LOGIN_H_

#include "./Message.h"

const static std::string CLIENT_LOGIN_REQUEST_PREFIX = "LOGIN_REQUEST";
const static std::string SERVER_LOGIN_RESPONSE_PREFIX = "LOGIN_RESPONSE";

enum LoginResponseCode
{
    LOGIN_SUCCESS = 1,
    LOGIN_FAILED = 2
};

struct LoginRequestMessage : public Message
{
    LoginRequestMessage(const int &connection_id, const std::string &username, const std::string &password) : Message(connection_id, username, MessageTarget::CLIENT_TO_SERVER, CLIENT_LOGIN_REQUEST_PREFIX + " " + username + " " + password) {}
};

struct LoginResponseMessage : public Message
{
    LoginResponseMessage(const int &connection_id, const std::string &username, const LoginResponseCode &response_code) : Message(connection_id, username, SERVER_TO_CLIENT, SERVER_LOGIN_RESPONSE_PREFIX + " " + std::to_string(response_code)) {}
};

#endif // _LOGIN_H_