#include "./Database.h"
#include <fstream>

/**
 * Default constructor for Database.
 * Initializes the Database with a default file path.
 */
Database::Database() : Database("./database.txt") {}

/**
 * Constructor for Database with a specified file path.
 * Initializes the Database and loads data from the given path.
 *
 * @param path The file path to the database file.
 */
Database::Database(std::string path) : path(path)
{
    load();
}

/**
 * Adds a new user to the database.
 *
 * @param username The username of the new user.
 * @param password The password for the new user.
 * @return True if the user was added successfully, False if the user already exists.
 */
bool Database::add(std::string username, std::string password)
{
    if (!contains(username))
    {
        data.push_back(User(username, password));
        save();
        return true;
    }
    return false;
}

/**
 * Checks if a user exists in the database by username.
 *
 * @param username The username to check.
 * @return True if the user exists, False otherwise.
 */
bool Database::contains(std::string username)
{
    return std::any_of(data.begin(), data.end(), [&username](const User &user)
                       { return user.getUsername() == username; });
}

/**
 * Checks if a user exists in the database by username and password.
 *
 * @param username The username to check.
 * @param password The password to check.
 * @return True if the user exists and the password matches, False otherwise.
 */
bool Database::contains(std::string username, std::string password)
{
    return std::any_of(data.begin(), data.end(), [&username, &password](const User &user)
                       { return user.getUsername() == username && user.getPassword() == password; });
}

/**
 * Removes a user from the database by username.
 *
 * @param username The username of the user to remove.
 */
void Database::remove(std::string username)
{
    data.erase(std::remove_if(data.begin(), data.end(), [&username](const User &user)
                              { return user.getUsername() == username; }),
               data.end());
    save();
}

/**
 * Updates a user's password in the database.
 *
 * @param username The username of the user.
 * @param password The new password for the user.
 */
void Database::update(std::string username, std::string password)
{
    auto it = std::find_if(data.begin(), data.end(), [&username](const User &user)
                           { return user.getUsername() == username; });
    if (it != data.end())
    {
        it->setPassword(password);
    }
    save();
}

/**
 * Saves the current state of the database to the file.
 * Serializes each user and writes them to the database file.
 */
void Database::save()
{
    std::ofstream file(path);
    if (file.is_open())
    {
        for (const auto &user : data)
        {
            file << user.serialize() << std::endl;
        }
        file.close();
    }
}

/**
 * Loads the database from the file.
 * Deserializes each line of the file into a user and adds them to the database.
 */
void Database::load()
{
    std::ifstream file(path);
    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            data.push_back(User::deserialize(line));
        }
        file.close();
    }
}
