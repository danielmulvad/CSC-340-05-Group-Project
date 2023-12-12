#include "./Message.h"

/**
 * Constructs a new Message with the current timestamp.
 *
 * @param connectionId The ID of the connection from which the message originates.
 * @param username The username of the message sender.
 * @param target The target audience of the message.
 * @param msg The message content.
 */
Message::Message(const int &connectionId, const std::string &username, const MessageTarget &target, const std::string &msg) : connectionId(connectionId), username(username), target(target), content(msg), timestamp(getCurrentTimestamp()) {}

/**
 * Constructs a new Message with a specified timestamp.
 *
 * @param connectionId The ID of the connection from which the message originates.
 * @param username The username of the message sender.
 * @param target The target audience of the message.
 * @param ts The timestamp of the message.
 * @param msg The message content.
 */
Message::Message(const int &connectionId, const std::string &username, const MessageTarget &target, const std::string &ts, const std::string &msg) : connectionId(connectionId), username(username), target(target), content(msg), timestamp(ts) {}

/**
 * Serializes the message into a string format.
 *
 * @return A string representing the serialized form of the message.
 */
std::string Message::serialize() const
{
    std::string connectionId = std::to_string(this->connectionId);
    std::string target = std::to_string(this->target);
    return connectionId + ";" + username + ";" + target + ";" + timestamp + ";" + content;
}

/**
 * Deserializes a string into a Message object.
 *
 * @param serialized The string to be deserialized into a Message.
 * @return A Message object derived from the serialized string.
 * @throws std::runtime_error If the serialized string format is invalid.
 */
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

/**
 * Returns a string representation of the Message.
 *
 * @return A string that represents the message in a human-readable format.
 */
std::string Message::toString() const
{
    std::string connectionId = std::to_string(this->connectionId);
    return "[" + connectionId + "][" + this->timestamp + "] " + this->username + ": " + this->content;
}

/**
 * Gets the current timestamp in a human-readable format.
 *
 * @return A string representing the current timestamp in the format "YYYY-MM-DD HH:MM:SS".
 */
std::string Message::getCurrentTimestamp()
{
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}
