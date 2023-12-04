#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <iomanip>
#include <sstream>

std::string getCurrentTimestamp()
{
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

struct Message
{
    int user_id;
    std::string content;
    std::string timestamp;

    Message(const int &user_id, const std::string &msg) : user_id(user_id), content(msg), timestamp(getCurrentTimestamp()) {}
    Message(const int &user_id, const std::string &ts, const std::string &msg) : user_id(user_id), content(msg), timestamp(ts) {}

    std::string serialize() const
    {
        std::string user_id = std::to_string(this->user_id);
        return user_id + ";" + timestamp + ";" + content;
    }

    static Message deserialize(const std::string &serialized)
    {
        std::vector<std::string> parts;
        std::stringstream ss(serialized);
        std::string part;

        while (std::getline(ss, part, ';'))
        {
            parts.push_back(part);
        }

        const size_t expectedParts = 3;
        if (parts.size() == expectedParts)
        {
            int user_id = std::stoi(parts[0]);
            std::string timestamp = parts[1];
            std::string msg = parts[2];

            return Message(user_id, timestamp, msg);
        }
        printf("Invalid serialized message format for %s\n", serialized.c_str());
        throw std::runtime_error("Invalid serialized message format");
    }

    std::string toString() const
    {
        std::string user_id = std::to_string(this->user_id);
        return "[" + timestamp + "] " + user_id + ": " + content;
    }

    friend std::ostream &operator<<(std::ostream &os, const Message &msg)
    {
        os << msg.toString();
        return os;
    }

    friend bool operator==(const Message &lhs, const Message &rhs)
    {
        return lhs.timestamp == rhs.timestamp && lhs.content == rhs.content;
    }
};

// Server->Client message
const static std::string SERVER_TO_CLIENT_PREFIX = "_SERVER_TO_CLIENT_";
const static std::string CLIENT_TO_SERVER_PREFIX = "_CLIENT_TO_SERVER_";

// Establish Connection
const static std::string CLIENT_ESTABLISH_CONNECTION_REQUEST_PREFIX = "ESTABLISH_CONNECTION_REQUEST";
const static std::string SERVER_ESTABLISH_CONNECTION_RESPONSE_PREFIX = "ESTABLISH_CONNECTION_RESPONSE";
struct EstablishConnectionRequestMessage : public Message
{
    EstablishConnectionRequestMessage(const int &connection_id) : Message(connection_id, CLIENT_TO_SERVER_PREFIX + " " + CLIENT_ESTABLISH_CONNECTION_REQUEST_PREFIX) {}
};
struct EstablishConnectionResponseMessage : public Message
{
    EstablishConnectionResponseMessage(const int &connection_id) : Message(connection_id, SERVER_TO_CLIENT_PREFIX + " " + SERVER_ESTABLISH_CONNECTION_RESPONSE_PREFIX + " " + std::to_string(connection_id)) {}
};

// Drop Connection
const static std::string CLIENT_DROP_CONNECTION_REQUEST_PREFIX = "DROP_CONNECTION_REQUEST";
const static std::string SERVER_DROP_CONNECTION_RESPONSE_PREFIX = "DROP_CONNECTION_RESPONSE";
struct DropConnectionRequestMessage : public Message
{
    DropConnectionRequestMessage(const int &connection_id) : Message(connection_id, CLIENT_TO_SERVER_PREFIX + " " + CLIENT_DROP_CONNECTION_REQUEST_PREFIX) {}
};
struct DropConnectionResponseMessage : public Message
{
    DropConnectionResponseMessage(const int &connection_id) : Message(connection_id, SERVER_TO_CLIENT_PREFIX + " " + SERVER_DROP_CONNECTION_RESPONSE_PREFIX + " " + std::to_string(connection_id)) {}
};

// Register
const static std::string CLIENT_REGISTER_REQUEST_PREFIX = "REGISTER_REQUEST";
const static std::string SERVER_REGISTER_RESPONSE_PREFIX = "REGISTER_RESPONSE";
struct RegisterRequestMessage : public Message
{
    RegisterRequestMessage(const int &connection_id, const std::string &username, const std::string &password) : Message(connection_id, CLIENT_TO_SERVER_PREFIX + " " + CLIENT_REGISTER_REQUEST_PREFIX + " " + username + " " + password) {}
};
enum RegisterResponseCode
{
    REGISTER_SUCCESS = 1,
    REGISTER_FAILED = 2
};
struct RegisterResponseMessage : public Message
{
    RegisterResponseMessage(const int &connection_id, const RegisterResponseCode &response_code) : Message(connection_id, SERVER_TO_CLIENT_PREFIX + " " + SERVER_REGISTER_RESPONSE_PREFIX + " " + std::to_string(response_code)) {}
};

// Login
const static std::string CLIENT_LOGIN_REQUEST_PREFIX = "LOGIN_REQUEST";
const static std::string SERVER_LOGIN_RESPONSE_PREFIX = "LOGIN_RESPONSE";
struct LoginRequestMessage : public Message
{
    LoginRequestMessage(const int &connection_id, const std::string &username, const std::string &password) : Message(connection_id, CLIENT_TO_SERVER_PREFIX + " " + CLIENT_LOGIN_REQUEST_PREFIX + " " + username + " " + password) {}
};
enum LoginResponseCode
{
    LOGIN_SUCCESS = 1,
    LOGIN_FAILED = 2
};
struct LoginResponseMessage : public Message
{
    LoginResponseMessage(const int &connection_id, const LoginResponseCode &response_code) : Message(connection_id, SERVER_TO_CLIENT_PREFIX + " " + SERVER_LOGIN_RESPONSE_PREFIX + " " + std::to_string(response_code)) {}
};

#endif