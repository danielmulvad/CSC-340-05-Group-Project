#include "./User.h"
#include <sstream>

User::User(std::string username, std::string password) : username(username), password(password) {}

std::string User::getUsername() const { return username; }
void User::setUsername(std::string username) { this->username = username; }
std::string User::getPassword() const { return password; }
void User::setPassword(std::string password) { this->password = password; }

std::string User::serialize() const
{
    return username + ";" + password;
}

User User::deserialize(const std::string &str)
{
    std::string username, password;

    std::istringstream iss(str);
    std::getline(iss, username, ';');
    std::getline(iss, password, ';');

    return User(username, password);
}
