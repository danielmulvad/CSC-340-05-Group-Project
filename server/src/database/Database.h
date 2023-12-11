#ifndef _DATABASE_H_
#define _DATABASE_H_

#include "./User.h"

class Database
{
private:
    std::string path;
    std::vector<User> data;

public:
    Database();
    Database(std::string path);

    bool add(std::string username, std::string password);
    bool contains(std::string username, std::string password);
    bool contains(std::string username);
    void load();
    void remove(std::string username);
    void save();
    void update(std::string username, std::string password);
};

#endif // _DATABASE_H_