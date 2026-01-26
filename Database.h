#ifndef DATABASE_H
#define DATABASE_H
#include <vector>
#include "Models.h"
#include "User.h"
#include "Student.h"
#include "Teacher.h"

class Database {
public:
    vector<User> users;
    vector<Student> students;
    vector<Teacher> teachers;
    vector<ClassInfo> classes;
    vector<Session> sessions;
    vector<Attendance> attendances;

    void loadAllData();
    void saveSessions();
    void saveAttendance();
    void saveUsers();

    string getStudentIDByUserID(string uID);
    string getTeacherIDByUserID(string uID);
    string getUserIDByRoleID(string rID);
    vector<string> split(string s, char d);

private:
    void loadUsers(); void loadStudents(); void loadTeachers();
    void loadClasses(); void loadSessions(); void loadAttendance();
};
#endif