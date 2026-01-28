#include "Database.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

vector<string> Database::split(string str, char delimiter) {
    vector<string> tokens;
    string token;
    stringstream ss(str);

    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

void Database::loadAllData() {
    loadUsers();
    loadStudents();
    loadTeachers();
    loadClasses();
    loadSessions();
    loadAttendance();

    cout << ">> Da tai du lieu xong.\n";
}

void Database::loadUsers() {
    users.clear();
    ifstream file("users.txt");
    string line;

    while (getline(file, line)) {
        vector<string> data = split(line, '|');
        if (data.size() >= 8) {
            users.push_back(User(
                data[0], data[1], data[2], data[3],
                data[4], data[5], data[6], data[7]
            ));
        }
    }
    file.close();
}

void Database::loadStudents() {
    students.clear();
    ifstream f("students.txt");
    string line;

    while (getline(f, line)) {
        vector<string> d = split(line, '|');
        if (d.size() >= 3) {
            students.push_back(Student(d[0], d[1], d[2]));
        }
    }
    f.close();
}

void Database::loadTeachers() {
    teachers.clear();
    ifstream f("teachers.txt");
    string line;

    while (getline(f, line)) {
        vector<string> d = split(line, '|');
        if (d.size() >= 4) {
            teachers.push_back(Teacher(d[0], d[1], d[2], d[3]));
        }
    }
    f.close();
}

void Database::loadClasses() {
    classes.clear();
    ifstream f("classes.txt");
    string line;

    while (getline(f, line)) {
        vector<string> d = split(line, '|');
        if (d.size() >= 4) {
            classes.push_back(ClassInfo(d[0], d[1], d[2], d[3]));
        }
    }
    f.close();
}

void Database::loadSessions() {
    sessions.clear();
    ifstream f("sessions.txt");
    string line;

    while (getline(f, line)) {
        vector<string> d = split(line, '|');
        if (d.size() >= 8) {
            sessions.push_back(Session(
                d[0], d[1], d[2], d[3],
                d[4], d[5],
                d[6] == "1",
                d[7]
            ));
        }
    }
    f.close();
}

void Database::loadAttendance() {
    attendances.clear();
    ifstream f("attendance.txt");
    string line;

    while (getline(f, line)) {
        vector<string> d = split(line, '|');
        if (d.size() >= 4) {
            attendances.push_back(Attendance(d[0], d[1], d[2], d[3]));
        }
    }
    f.close();
}

string Database::getStudentIDByUserID(string uID) {
    for (int i = 0; i < students.size(); i++)
        if (students[i].getUserID() == uID)
            return students[i].getStudentID();
    return "";
}

string Database::getTeacherIDByUserID(string uID) {
    for (int i = 0; i < teachers.size(); i++)
        if (teachers[i].getUserID() == uID)
            return teachers[i].getTeacherID();
    return "";
}

string Database::getUserIDByRoleID(string rID) {
    for (int i = 0; i < students.size(); i++)
        if (students[i].getStudentID() == rID)
            return students[i].getUserID();

    for (int i = 0; i < teachers.size(); i++)
        if (teachers[i].getTeacherID() == rID)
            return teachers[i].getUserID();

    return "";
}

void Database::saveSessions() {
    ofstream f("sessions.txt");

    for (int i = 0; i < sessions.size(); i++) {
        f << sessions[i].getSessionID() << "|"
          << sessions[i].getClassID() << "|"
          << sessions[i].getTeacherID() << "|"
          << sessions[i].getDate() << "|"
          << sessions[i].getStartTime() << "|"
          << sessions[i].getEndTime() << "|"
          << (sessions[i].getIsOpen() ? "1" : "0") << "|"
          << sessions[i].getPassword()
          << endl;
    }
    f.close();
}

void Database::saveAttendance() {
    ofstream f("attendance.txt");

    for (int i = 0; i < attendances.size(); i++) {
        f << attendances[i].getStudentID() << "|"
          << attendances[i].getSessionID() << "|"
          << attendances[i].getStatus() << "|"
          << attendances[i].getCheckinTime()
          << endl;
    }
    f.close();
}

void Database::saveUsers() {
    ofstream file("users.txt");

    for (int i = 0; i < users.size(); i++) {
        file << users[i].getUserID() << "|"
             << users[i].getUsername() << "|"
             << users[i].getPassword() << "|"
             << users[i].getFullName() << "|"
             << users[i].getDob() << "|"
             << users[i].getGender() << "|"
             << users[i].getPhone() << "|"
             << users[i].getRole()
             << endl;
    }
    file.close();
}
