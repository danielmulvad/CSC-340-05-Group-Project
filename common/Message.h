#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

struct Message
{
    std::string timestamp;
    std::string content;

    Message(const std::string &ts, const std::string &msg) : timestamp(ts), content(msg) {}

    std::string serialize() const
    {
        return timestamp + ";" + content;
    }

    static Message deserialize(const std::string &serialized)
    {
        auto separatorPos = serialized.find(';');
        if (separatorPos != std::string::npos)
        {
            std::string ts = serialized.substr(0, separatorPos);
            std::string msg = serialized.substr(separatorPos + 1);
            return Message(ts, msg);
        }
        return Message("", "");
    }
};

std::string getCurrentTimestamp()
{
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

#endif