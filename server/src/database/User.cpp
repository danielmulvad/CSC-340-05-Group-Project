#include "./User.h"
#include <sstream>

/**
 * Constructor for User.
 * Initializes a user with a username and password.
 *
 * @param username The username of the user.
 * @param password The password of the user.
 */
User::User(std::string username, std::string password) : username(username), password(password) {}

/**
 * Gets the username of the user.
 *
 * @return The username as a string.
 */
std::string User::getUsername() const { return username; }

/**
 * Sets the username of the user.
 *
 * @param username The new username to set.
 */
void User::setUsername(std::string username) { this->username = username; }

/**
 * Gets the password of the user.
 *
 * @return The password as a string.
 */
std::string User::getPassword() const { return password; }

/**
 * Sets the password of the user.
 *
 * @param password The new password to set.
 */
void User::setPassword(std::string password) { this->password = password; }

/**
 * Serializes the user into a string.
 * Converts the user's data into a string format, typically for storage or transmission.
 *
 * @return A string representing the serialized user.
 */
std::string User::serialize() const
{
    return username + ";" + password;
}

/**
 * Deserializes a string into a User object.
 * Converts a string back into a User object. This is typically used when loading user data from storage.
 *
 * @param str The string to deserialize into a User object.
 * @return A User object derived from the serialized string.
 */
User User::deserialize(const std::string &str)
{
    std::string username, password;

    std::istringstream iss(str);
    std::getline(iss, username, ';');
    std::getline(iss, password, ';');

    return User(username, password);
}
