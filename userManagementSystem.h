#pragma once
#ifndef USER_MANAGEMENT_SYSTEM_H
#define USER_MANAGEMENT_SYSTEM_H

#include "user.h"

class UserManagementSystem {
private:
    User* head;
    User* prev;
    string loggedInUser;

    void deleteUsers();
    void initializeAdminUser();
    void loadUsersFromFile();
    void saveUsersToFile();
    void addUser(const string& username, const string& password);
    void deleteUser(const string& username);
    void changePassword();
    void viewProfile() const;

public:
    UserManagementSystem();
    ~UserManagementSystem();

    void login(const string& enteredUsername, const string& enteredPassword);
    void logout();
    void displayUsers() const;
    void displayMenu();
    void adminMenu() const;
    void userMenu() const;
    void addUserMenu();
    void deleteUserMenu();
};
#endif // USER_MANAGEMENT_SYSTEM_H
