#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct User {
    string username;
    string password;
    User* next;

    User(const string& uname, const string& pwd) : username(uname), password(pwd), next(nullptr) {}
};

class UserManagementSystem {
private:
    User* head;
    string loggedInUser;

public:
    UserManagementSystem() : head(nullptr) {
        initializeAdminUser();
        loadUsersFromFile();
    }

    ~UserManagementSystem() {
        saveUsersToFile();
        deleteUsers();
    }
    void initializeAdminUser() {
        addUser("admin", "adminpass");
    }

    void loadUsersFromFile() {
        ifstream file("users.txt");
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                size_t pos = line.find(':');
                if (pos != string::npos) {
                    string username = line.substr(0, pos);
                    string password = line.substr(pos + 1);
                    addUser(username, password);
                }
            }
            file.close();
        }
    }

    void saveUsersToFile() {
        ofstream file("users.txt");
        if (file.is_open()) {
            User* current = head;
            while (current != nullptr) {
                file << current->username << ":" << current->password << "\n";
                current = current->next;
            }
            file.close();
        }
    }

    void addUser(const string& username, const string& password) {
        User* newUser = new User(username, password);
        newUser->next = head;
        head = newUser;
    }

    void deleteUsers() {
        while (head != nullptr) {
            User* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void login(const string& enteredUsername, const string& enteredPassword) {
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

    void logout() {
        loggedInUser = "";
        cout << "Logged out successfully.\n";
    }

    void displayUsers() const {
        User* current = head;
        while (current != nullptr) {
            cout << "Username: " << current->username << "\n";
            current = current->next;
        }
    }

    void displayMenu() {
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
                    case 1:
                        addUserMenu();
                        break;
                    case 2:
                        deleteUserMenu();
                        break;
                    case 3:
                        viewProfile();
                        break;
                    case 4:
                        displayUsers();
                        break;
                    case 5:
                        logout();
                        return;
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
                    case 1:
                        changePassword();
                        break;
                    case 2:
                        viewProfile();
                        break;
                    case 3:
                        logout();
                        return;
                    default:
                        cout << "Invalid choice. Try again.\n";
                        break;
                    }
                }
            }
        }
    }

    void adminMenu() const {
        cout << "Admin Menu:\n";
        cout << "1. Add User\n";
        cout << "2. Delete User\n";
        cout << "3. View Profile\n";
        cout << "4. Display Users\n";
        cout << "5. Logout\n";
    }

    void userMenu() const {
        cout << "User Menu:\n";
        cout << "1. Change Password\n";
        cout << "2. View Profile\n";
        cout << "3. Logout\n";
    }

    void addUserMenu() {
        string username, password;
        cout << "Enter new username: ";
        cin >> username;
        cout << "Enter password for " << username << ": ";
        cin >> password;
        addUser(username, password);
    }

    void deleteUserMenu() {
        string usernameToDelete;
        cout << "Enter username to delete: ";
        cin >> usernameToDelete;
        deleteUser(usernameToDelete);
    }

    void deleteUser(const string& username) {
        if (loggedInUser == "admin") {
            User* current = head;
            User* prev = nullptr;

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
                    return;
                }

                prev = current;
                current = current->next;
            }

            cout << "User " << username << " not found.\n";
        }
        else {
            cout << "You do not have permission to delete users.\n";
        }
    }

    void changePassword() {
        if (loggedInUser == "admin") {
            cout << "Admins cannot change their password.\n";
        }
        else {
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
    }

    void viewProfile() const {
        cout << "Profile information for " << loggedInUser << ":\n";
        // Add more profile information display if needed
    }
};

int main() {
    UserManagementSystem system;
    system.displayMenu();
    return 0;
}
