#pragma once
#ifndef USER_H
#define USER_H

#include <string>
using namespace std;
class User {
public:
    string username;
    string password;
    User* next;
    User* prev;

    User(const string& uname, const string& pword);
};

#endif // USER_H

