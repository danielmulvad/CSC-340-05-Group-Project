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

enum MessageTarget
{
    SERVER_TO_CLIENT = 1,
    CLIENT_TO_SERVER = 2,
    BROADCAST = 3
};

struct Message
{
    int user_id;
    MessageTarget target;
    std::string content;
    std::string timestamp;

    Message(const int &user_id, const MessageTarget &target, const std::string &msg) : user_id(user_id), target(target), content(msg), timestamp(getCurrentTimestamp()) {}
    Message(const int &user_id, const MessageTarget &target, const std::string &ts, const std::string &msg) : user_id(user_id), target(target), content(msg), timestamp(ts) {}

    std::string serialize() const
    {
        std::string user_id = std::to_string(this->user_id);
        std::string target = std::to_string(this->target);
        return user_id + ";" + target + ";" + timestamp + ";" + content;
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

        const size_t expectedParts = 4;
        if (parts.size() == expectedParts)
        {
            int user_id = std::stoi(parts[0]);
            MessageTarget message_target = static_cast<MessageTarget>(std::stoi(parts[1]));
            std::string timestamp = parts[2];
            std::string msg = parts[3];

            return Message(user_id, message_target, timestamp, msg);
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
    EstablishConnectionRequestMessage(const int &connection_id) : Message(connection_id, MessageTarget::CLIENT_TO_SERVER, CLIENT_ESTABLISH_CONNECTION_REQUEST_PREFIX) {}
};
struct EstablishConnectionResponseMessage : public Message
{
    EstablishConnectionResponseMessage(const int &connection_id) : Message(connection_id, MessageTarget::SERVER_TO_CLIENT, SERVER_ESTABLISH_CONNECTION_RESPONSE_PREFIX + " " + std::to_string(connection_id)) {}
};

// Drop Connection
const static std::string CLIENT_DROP_CONNECTION_REQUEST_PREFIX = "DROP_CONNECTION_REQUEST";
const static std::string SERVER_DROP_CONNECTION_RESPONSE_PREFIX = "DROP_CONNECTION_RESPONSE";
struct DropConnectionRequestMessage : public Message
{
    DropConnectionRequestMessage(const int &connection_id) : Message(connection_id, MessageTarget::CLIENT_TO_SERVER, CLIENT_DROP_CONNECTION_REQUEST_PREFIX) {}
};
struct DropConnectionResponseMessage : public Message
{
    DropConnectionResponseMessage(const int &connection_id) : Message(connection_id, MessageTarget::SERVER_TO_CLIENT, SERVER_DROP_CONNECTION_RESPONSE_PREFIX + " " + std::to_string(connection_id)) {}
};

// Register
const static std::string CLIENT_REGISTER_REQUEST_PREFIX = "REGISTER_REQUEST";
const static std::string SERVER_REGISTER_RESPONSE_PREFIX = "REGISTER_RESPONSE";
struct RegisterRequestMessage : public Message
{
    RegisterRequestMessage(const int &connection_id, const std::string &username, const std::string &password) : Message(connection_id, MessageTarget::CLIENT_TO_SERVER, CLIENT_REGISTER_REQUEST_PREFIX + " " + username + " " + password) {}
};
enum RegisterResponseCode
{
    REGISTER_SUCCESS = 1,
    REGISTER_FAILED = 2
};
struct RegisterResponseMessage : public Message
{
    RegisterResponseMessage(const int &connection_id, const RegisterResponseCode &response_code) : Message(connection_id, MessageTarget::SERVER_TO_CLIENT, SERVER_REGISTER_RESPONSE_PREFIX + " " + std::to_string(response_code)) {}
};

// Login
const static std::string CLIENT_LOGIN_REQUEST_PREFIX = "LOGIN_REQUEST";
const static std::string SERVER_LOGIN_RESPONSE_PREFIX = "LOGIN_RESPONSE";
struct LoginRequestMessage : public Message
{
    LoginRequestMessage(const int &connection_id, const std::string &username, const std::string &password) : Message(connection_id, MessageTarget::CLIENT_TO_SERVER, CLIENT_LOGIN_REQUEST_PREFIX + " " + username + " " + password) {}
};
enum LoginResponseCode
{
    LOGIN_SUCCESS = 1,
    LOGIN_FAILED = 2
};
struct LoginResponseMessage : public Message
{
    LoginResponseMessage(const int &connection_id, const LoginResponseCode &response_code) : Message(connection_id, SERVER_TO_CLIENT, SERVER_LOGIN_RESPONSE_PREFIX + " " + std::to_string(response_code)) {}
};

// Broadcast
const static std::string CLIENT_BROADCAST_REQUEST_PREFIX = "BROADCAST_REQUEST";
const static std::string SERVER_BROADCAST_RESPONSE_PREFIX = "BROADCAST_RESPONSE";
struct BroadcastRequestMessage : public Message
{
    BroadcastRequestMessage(const int &connection_id, const std::string &msg) : Message(connection_id, MessageTarget::CLIENT_TO_SERVER, CLIENT_BROADCAST_REQUEST_PREFIX + " " + msg) {}
};
struct BroadcastResponseMessage : public Message
{
    BroadcastResponseMessage(const int &connection_id, const std::string &msg) : Message(connection_id, MessageTarget::BROADCAST, SERVER_BROADCAST_RESPONSE_PREFIX + " " + msg) {}
};

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

#endif