#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <iomanip>
#include <sstream>

// Server->Client message
const static std::string SERVER_TO_CLIENT_PREFIX = "_SERVER_TO_CLIENT_";
const static std::string CLIENT_TO_SERVER_PREFIX = "_CLIENT_TO_SERVER_";

// Establish connection
const static std::string CLIENT_ESTABLISH_CONNECTION_REQUEST_PREFIX = "ESTABLISH_CONNECTION_REQUEST";
const static std::string SERVER_ESTABLISH_CONNECTION_RESPONSE_PREFIX = "ESTABLISH_CONNECTION_RESPONSE";

// Drop connection
const static std::string CLIENT_DROP_CONNECTION_REQUEST_PREFIX = "DROP_CONNECTION_REQUEST";
const static std::string SERVER_DROP_CONNECTION_RESPONSE_PREFIX = "DROP_CONNECTION_RESPONSE";

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

#endif