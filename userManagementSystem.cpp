#include "userManagementSystem.h"
#include <iostream>
#include <fstream>

enum AdminUserAction { LOGOUT, ADD_USER, DELETE_USER, VIEW_PROFILE, DISPLAY_USER, CHANGE_PASS, COMMUNICATE };

UserManagementSystem::UserManagementSystem() : head(nullptr), loggedInUser("") {}

UserManagementSystem::~UserManagementSystem() {
    deleteUsers();
}

void UserManagementSystem::deleteUsers() {
    while (head != nullptr) {
        User* temp = head;
        head = head->next;
        delete temp;
    }
    head = nullptr;
}

void UserManagementSystem::initializeAdminUser() {
    addUser("admin", "adminpass");
}

void UserManagementSystem::loadUsersFromFile() {
    ifstream file("users.txt");
    if (!file.is_open()) {
        cerr << "Error opening file 'users.txt' for reading.\n";
        return;
    }

    string line;
    while (getline(file, line)) {
        size_t pos = line.find('-');
        if (pos != string::npos) {
            string username = line.substr(0, pos);
            string password = line.substr(pos + 1);
            addUser(username, password);
        }
    }

    file.close();
}

void UserManagementSystem::saveUsersToFile() {
    ofstream file("users.txt");
    if (!file.is_open()) {
        cerr << "Error opening file 'users.txt' for writing.\n";
        return;
    }

    User* current = head;
    while (current != nullptr) {
        file << current->username << "|" << current->password << "\n";
        current = current->next;
    }

    file.close();
}

void UserManagementSystem::addUser(const string& username, const string& password) {
    User* newUser = new User(username, password);
    newUser->next = head;
    head = newUser;
}

void UserManagementSystem::deleteUser(const string& username) {
    if (loggedInUser != "admin") {
        cout << "You do not have permission to delete users.\n";
        return;
    }

    User* current = head;
    User* prev = nullptr;

    ofstream outputFile("temp.txt");
    if (!outputFile.is_open()) {
        cerr << "Error opening file 'temp.txt' for writing.\n";
        return;
    }

    while (current != nullptr) {
        if (current->username == username) {
            if (prev != nullptr) {
                prev->next = current->next;
            }
            else {
                head = current->next;
            }

            cout << "User " << username << " deleted successfully.\n";
            delete current;
            break;
        }
        else {
            outputFile << current->username << ":" << current->password << "\n";
        }

        prev = current;
        current = current->next;
    }

    outputFile.close();

    if (remove("users.txt") != 0 || rename("temp.txt", "users.txt") != 0) {
        cerr << "Error updating user file.\n";
    }
}

void UserManagementSystem::changePassword() {
    if (loggedInUser == "admin") {
        cout << "Admins cannot change their password.\n";
        return;
    }

    string newPassword;
    cout << "Enter new password: ";
    cin >> newPassword;

    User* current = head;
    while (current != nullptr) {
        if (current->username == loggedInUser) {
            current->password = newPassword;
            cout << "Password changed successfully.\n";
            return;
        }
        current = current->next;
    }
}

void UserManagementSystem::viewProfile() const {
std:cout << "Profile information for " << loggedInUser << ":\n";
    // Add more profile information display if needed
}

void UserManagementSystem::login(const string& enteredUsername, const string& enteredPassword) {
    User* current = head;
    while (current != nullptr) {
        if (current->username == enteredUsername && current->password == enteredPassword) {
            loggedInUser = enteredUsername;
            cout << "Welcome, " << loggedInUser << "!\n";
            return;
        }
        current = current->next;
    }
    cout << "Invalid credentials. Login failed.\n";
}

void UserManagementSystem::logout() {
    loggedInUser = "";
    cout << "Logged out successfully.\n";
}

void UserManagementSystem::displayUsers() const {
    User* current = head;
    while (current != nullptr) {
        cout << "Username: " << current->username << "\n";
        current = current->next;
    }
}

void UserManagementSystem::displayMenu() {
    initializeAdminUser();
    loadUsersFromFile();

    while (true) {
        string username, password;

        if (loggedInUser.empty()) {
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            login(username, password);

            if (loggedInUser.empty()) {
                cout << "Login unsuccessful. Please try again.\n";
                continue;
            }
        }

        if (!loggedInUser.empty()) {
            cout << "Login successful.\n";

            if (loggedInUser == "admin") {
                // Admin menu
                adminMenu();
                int choice;
                cout << "Enter your choice: ";
                cin >> choice;

                switch (choice) {
                case LOGOUT:
                    logout();
                    saveUsersToFile();
                    return;
                case ADD_USER:
                    addUserMenu();
                    break;
                case DELETE_USER:
                    deleteUserMenu();
                    break;
                case VIEW_PROFILE:
                    viewProfile();
                    break;
                case DISPLAY_USER:
                    displayUsers();
                    break;
                case COMMUNICATE:
                    cout << "Coming Soon!" << endl;
                    break;
                default:
                    cout << "Invalid choice. Try again.\n";
                    break;
                }
            }
            else {
                // Regular user menu
                userMenu();
                int choice;
                cout << "Enter your choice: ";
                cin >> choice;

                switch (choice) {
                case LOGOUT:
                    logout();
                    return;
                case CHANGE_PASS:
                    changePassword();
                    break;
                case VIEW_PROFILE:
                    viewProfile();
                    break;
                case COMMUNICATE:
                    cout << "Coming Soon!" << endl;
                default:
                    cout << "Invalid choice. Try again.\n";
                    break;
                }
            }
        }
    }
}

void UserManagementSystem::adminMenu() const {
    cout << "--------------------------------" << endl;
    cout << "Admin Menu:\n";
    cout << "--------------------------------" << endl;
    cout << "   |" << LOGOUT << "|" << " Logout\n";
    cout << "   |" << ADD_USER << "|" << " Add User\n";
    cout << "   |" << DELETE_USER << "|" << " Delete User\n";
    cout << "   |" << VIEW_PROFILE << "|" << " View Profile\n";
    cout << "   |" << DISPLAY_USER << "|" << " Display Users\n";
    cout << "   |" << COMMUNICATE << "|" << " Communicate with server\n";
    cout << "--------------------------------" << endl;

}

void UserManagementSystem::userMenu() const {
    cout << "--------------------------------" << endl;
    cout << "User Menu:\n";
    cout << "--------------------------------" << endl;
    cout << "   |" << LOGOUT << "|" << " Logout\n";
    cout << "   |" << CHANGE_PASS << "|" << " Change Password\n";
    cout << "   |" << VIEW_PROFILE << "|" << " View Profile\n";
    cout << "   |" << COMMUNICATE << "|" << " Communicate with server\n";
    cout << "--------------------------------" << endl;

}

void UserManagementSystem::addUserMenu() {
    string username, password;
    cout << "Enter new username: ";
    cin >> username;
    cout << "Enter password for " << username << ": ";
    cin >> password;
    addUser(username, password);
}

void UserManagementSystem::deleteUserMenu() {
    string usernameToDelete;
    cout << "Enter username to delete: ";
    cin >> usernameToDelete;
    if (loggedInUser == "admin") {
        deleteUser(usernameToDelete);
    }
    else {
        cout << "You do not have permission to delete users.\n";
    }
}
