#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <iomanip>
#include <sstream>

enum MessageTarget
{
    SERVER_TO_CLIENT = 1,
    CLIENT_TO_SERVER = 2,
    BROADCAST = 3
};

struct Message
{
public:
    int connectionId;
    std::string username;
    MessageTarget target;
    std::string content;
    std::string timestamp;

    Message(const int &connectionId, const std::string &username, const MessageTarget &target, const std::string &msg);
    Message(const int &connectionId, const std::string &username, const MessageTarget &target, const std::string &ts, const std::string &msg);

    std::string serialize() const;
    static Message deserialize(const std::string &serialized);

    std::string toString() const;

    friend std::ostream &operator<<(std::ostream &os, const Message &msg)
    {
        os << msg.toString();
        return os;
    }

    friend bool operator==(const Message &lhs, const Message &rhs)
    {
        return lhs.username == rhs.username && lhs.timestamp == rhs.timestamp && lhs.content == rhs.content;
    }

private:
    std::string getCurrentTimestamp();
};

#endif // _MESSAGE_H_