#ifndef USER_H
#define USER_H

#include <string>
#include <iostream>

using namespace std;

class Database; 

class User {
protected:
    string userID, username, password, fullName, dob, gender, phone, role;

public:
    User() {}
    User(string uid, string uname, string pass, string fname, string d, string gen, string ph, string r);

    string getUserID() 
        { return userID; }
    string getUsername() 
        { return username; }
    string getPassword() 
        { return password; }
    string getFullName() 
        { return fullName; }
    string getDob() 
        { return dob; }
    string getGender() 
        { return gender; }
    string getPhone() 
        { return phone; }
    string getRole() 
        { return role; }
    
    void setPassword(string p) 
        { password = p; }
    bool checkPassword(string p)
        { return password == p; }

    //Ham giao dien
    string getCurrentTime();
    string promptSelectClass(Database& db);
    void viewPersonalInfo(Database& db, User user, string roleID);

    //Ham xu ly chinh
    static void run(Database& db);
    static bool login(Database& db, User& currentUser, string& currentRoleID);
    static void forgotPassword(Database& db);
};

#endif