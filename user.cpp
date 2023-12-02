#include "user.h"
#include <string>
using namespace std;

User::User(const string& uname, const string& pword) : username(uname), password(pword), next(nullptr), prev(nullptr) {}