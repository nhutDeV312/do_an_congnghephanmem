#ifndef STUDENT_H
#define STUDENT_H
#include "User.h" // Ke thua tu User

class Student : public User {
private: string studentID, major;
public:
    Student() {}
    Student(string sid, string uid, string m);
    string getStudentID() { return studentID; }
    string getMajor() { return major; }

    void showMenu(Database& db, User user);
    
private:
    void showClassMenu(Database& db, string classID);
    void takeAttendance(Database& db, string classID);
    void viewHistory(Database& db, string classID);
};
#endif