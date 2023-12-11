#include "./Message.h"

Message::Message(const int &user_id, const MessageTarget &target, const std::string &msg) : user_id(user_id), target(target), content(msg), timestamp(getCurrentTimestamp()) {}
Message::Message(const int &user_id, const MessageTarget &target, const std::string &ts, const std::string &msg) : user_id(user_id), target(target), content(msg), timestamp(ts) {}

std::string Message::serialize() const
{
    std::string user_id = std::to_string(this->user_id);
    std::string target = std::to_string(this->target);
    return user_id + ";" + target + ";" + timestamp + ";" + content;
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

std::string Message::toString() const
{
    std::string user_id = std::to_string(this->user_id);
    return "[" + timestamp + "] " + user_id + ": " + content;
}

std::string Message::getCurrentTimestamp()
{
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}
