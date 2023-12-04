#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>

class DatabaseItem
{
public:
    std::string serialize();
    static DatabaseItem deserialize(const std::string &);
};

class User : DatabaseItem
{
private:
    std::string username;
    std::string password;

public:
    User(std::string username, std::string password) : username(username), password(password) {}

    std::string getUsername() const { return username; }
    void setUsername(std::string username) { this->username = username; }
    std::string getPassword() const { return password; }
    void setPassword(std::string password) { this->password = password; }

    std::string serialize() const
    {
        return username + ";" + password;
    }

    static User deserialize(const std::string &str)
    {
        std::string username, password;

        std::istringstream iss(str);
        std::getline(iss, username, ';');
        std::getline(iss, password, ';');

        return User(username, password);
    }
};

class Database
{
private:
    std::string path;
    std::vector<User> data;

public:
    Database() : Database("./database.txt") {}
    Database(std::string path) : path(path)
    {
        load();
    }

    bool add(std::string username, std::string password)
    {
        if (!contains(username))
        {
            data.push_back(User(username, password));
            save();
            return true;
        }
        return false;
    }

    bool contains(std::string username)
    {
        return std::any_of(data.begin(), data.end(), [&username](const User &user)
                           { return user.getUsername() == username; });
    }

    bool contains(std::string username, std::string password)
    {
        return std::any_of(data.begin(), data.end(), [&username, &password](const User &user)
                           { return user.getUsername() == username && user.getPassword() == password; });
    }

    void remove(std::string username)
    {
        data.erase(std::remove_if(data.begin(), data.end(), [&username](const User &user)
                                  { return user.getUsername() == username; }),
                   data.end());
        save();
    }

    void update(std::string username, std::string password)
    {
        auto it = std::find_if(data.begin(), data.end(), [&username](const User &user)
                               { return user.getUsername() == username; });
        if (it != data.end())
        {
            it->setPassword(password);
        }
        save();
    }

    void save()
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

    void load()
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
};

#endif // _DATABASE_H_