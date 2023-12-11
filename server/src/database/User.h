#ifndef _USER_H_
#define _USER_H_

#include "./DatabaseItem.h"

class User : DatabaseItem
{
private:
    std::string username;
    std::string password;

public:
    User(std::string username, std::string password);

    std::string getUsername() const;
    void setUsername(std::string username);
    std::string getPassword() const;
    void setPassword(std::string password);

    std::string serialize() const;

    static User deserialize(const std::string &str);
};

#endif // _USER_H_