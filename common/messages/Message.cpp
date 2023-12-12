#include "./Message.h"

Message::Message(const int &connectionId, const std::string &username, const MessageTarget &target, const std::string &msg) : connectionId(connectionId), username(username), target(target), content(msg), timestamp(getCurrentTimestamp()) {}
Message::Message(const int &connectionId, const std::string &username, const MessageTarget &target, const std::string &ts, const std::string &msg) : connectionId(connectionId), username(username), target(target), content(msg), timestamp(ts) {}

std::string Message::serialize() const
{
    std::string connectionId = std::to_string(this->connectionId);
    std::string target = std::to_string(this->target);
    return connectionId + ";" + username + ";" + target + ";" + timestamp + ";" + content;
}

Message Message::deserialize(const std::string &serialized)
{
    std::vector<std::string> parts;
    std::stringstream ss(serialized);
    std::string part;

    while (std::getline(ss, part, ';'))
    {
        parts.push_back(part);
    }

    const size_t expectedParts = 5;
    if (parts.size() == expectedParts)
    {
        int connectionId = std::stoi(parts[0]);
        std::string username = parts[1];
        MessageTarget message_target = static_cast<MessageTarget>(std::stoi(parts[2]));
        std::string timestamp = parts[3];
        std::string msg = parts[4];

        return Message(connectionId, username, message_target, timestamp, msg);
    }
    printf("Invalid serialized message format for %s\n", serialized.c_str());
    throw std::runtime_error("Invalid serialized message format");
}

std::string Message::toString() const
{
    std::string connectionId = std::to_string(this->connectionId);
    return "[" + connectionId + "][" + this->timestamp + "] " + this->username + ": " + this->content;
}

std::string Message::getCurrentTimestamp()
{
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}
