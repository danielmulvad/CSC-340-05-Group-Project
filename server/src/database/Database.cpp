#include "./Database.h"
#include <fstream>

Database::Database() : Database("./database.txt") {}
Database::Database(std::string path) : path(path)
{
    load();
}

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

bool Database::contains(std::string username)
{
    return std::any_of(data.begin(), data.end(), [&username](const User &user)
                       { return user.getUsername() == username; });
}

bool Database::contains(std::string username, std::string password)
{
    return std::any_of(data.begin(), data.end(), [&username, &password](const User &user)
                       { return user.getUsername() == username && user.getPassword() == password; });
}

void Database::remove(std::string username)
{
    data.erase(std::remove_if(data.begin(), data.end(), [&username](const User &user)
                              { return user.getUsername() == username; }),
               data.end());
    save();
}

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
