#ifndef MODELS_H
#define MODELS_H
#include <string>
using namespace std;

class ClassInfo {
private: string classID, subjectID, semester, room;
public:
    ClassInfo() {}
    ClassInfo(string c, string s, string sem, string r) : classID(c), subjectID(s), semester(sem), room(r) {}
    string getClassID() { return classID; }
    string getSubjectID() { return subjectID; }
    string getRoom() { return room; }
};

class Session {
private: string sessionID, classID, teacherID, date, startTime, endTime, password; bool isOpen;
public:
    Session() {}
    Session(string sid, string cid, string tid, string d, string s, string e, bool o, string p) 
        : sessionID(sid), classID(cid), teacherID(tid), date(d), startTime(s), endTime(e), isOpen(o), password(p) {}
    
    string getSessionID() { return sessionID; }
    string getClassID() { return classID; }
    string getTeacherID() { return teacherID; }
    string getDate() { return date; }
    string getStartTime() { return startTime; }
    string getEndTime() { return endTime; }
    string getPassword() { return password; }
    bool getIsOpen() { return isOpen; }
    void setDate(string d) { date = d; }
    void setStartTime(string t) { startTime = t; }
    void setEndTime(string t) { endTime = t; }
    void setPassword(string p) { password = p; }
    void setIsOpen(bool o) { isOpen = o; }
    string getStatusString() { return isOpen ? "MO" : "DONG"; }
};

class Attendance {
private: string studentID, sessionID, status, checkinTime;
public:
    Attendance() {}
    Attendance(string st, string se, string sta, string t) : studentID(st), sessionID(se), status(sta), checkinTime(t) {}
    string getStudentID() { return studentID; }
    string getSessionID() { return sessionID; }
    string getStatus() { return status; }
    string getCheckinTime() { return checkinTime; }
    void setStatus(string s) { status = s; }
};
#endif