#ifndef USER_H
#define USER_H

#include <string>
#include <iostream>
#include <ctime>

using namespace std;

class Database; // Forward declaration

class User {
protected:
    // Them thuoc tinh dob (Day of Birth)
    string userID, username, password, fullName, dob, gender, phone, role;

public:
    User() {}
    // Update Constructor nhan them dob
    User(string uid, string uname, string pass, string fname, string d, string gen, string ph, string r);

    string getUserID() { return userID; }
    string getUsername() { return username; }
    string getPassword() { return password; }
    string getFullName() { return fullName; }
    string getDob() { return dob; } // Getter cho ngay sinh
    string getGender() { return gender; }
    string getPhone() { return phone; }
    string getRole() { return role; }
    
    void setPassword(string p) { password = p; }
    bool checkPassword(string inputPass) { return password == inputPass; }

    // --- VIEW FUNCTIONS ---
    string getCurrentTime();
    string promptSelectClass(Database& db);
    void viewPersonalInfo(Database& db, User user, string roleID);

    // --- SYSTEM FUNCTIONS ---
    static void run(Database& db);
    static bool login(Database& db, User& currentUser, string& currentRoleID);
    static void forgotPassword(Database& db);
};

#endif