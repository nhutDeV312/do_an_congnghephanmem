#ifndef USER_H
#define USER_H
#include <string>
#include <iostream>
#include <ctime>

using namespace std;

class Database; // Forward declaration

class User {
protected:
    string userID, username, password, fullName, gender, phone, role;

public:
    User() {}
    User(string uid, string uname, string pass, string fname, string gen, string ph, string r);
    
    // --- GETTERS (Cần thiết để Database lưu file) ---
    string getUserID() { return userID; }
    string getUsername() { return username; }
    string getPassword() { return password; } // <--- BẮT BUỘC PHẢI CÓ
    string getFullName() { return fullName; }
    string getGender() { return gender; }     // <--- BẮT BUỘC PHẢI CÓ
    string getPhone() { return phone; }
    string getRole() { return role; }
    
    bool checkPassword(string p) { return password == p; }
    void setPassword(string p) { password = p; }

    // --- VIEW FUNCTIONS ---
    string getCurrentTime();
    string promptSelectClass(Database& db);
    void viewPersonalInfo(Database& db, User user, string roleID);

    // --- SYSTEM FUNCTIONS ---
    static void run(Database& db);;
    static bool login(Database& db, User& currentUser, string& currentRoleID);
    static void forgotPassword(Database& db);
};
#endif