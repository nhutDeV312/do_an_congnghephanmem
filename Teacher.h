#ifndef TEACHER_H
#define TEACHER_H
#include "User.h" // Ke thua tu User

class Teacher : public User {
private: string teacherID, department, degree;
public:
    Teacher() {}
    Teacher(string tid, string uid, string dept, string deg);
    string getTeacherID() { return teacherID; }
    string getDepartment() { return department; }
    string getDegree() { return degree; }

    void showMenu(Database& db, User user);

private:
    void showClassMenu(Database& db, string classID);
    void createSession(Database& db, string classID);
    void editSession(Database& db, string classID);
    void viewReport(Database& db, string classID);
};
#endif