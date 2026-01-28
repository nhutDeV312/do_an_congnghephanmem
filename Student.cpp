#include "Student.h"
#include "Database.h"
#include <iostream>
#include <iomanip>

using namespace std;

//Chức năng hỗ trợ

string findLecturerName(Database& db, string classID) {
    string teacherID = "";
    for (int i = 0; i < db.sessions.size(); i++) {
        if (db.sessions[i].getClassID() == classID) {
            teacherID = db.sessions[i].getTeacherID(); break;
        }
    }
    if (teacherID == "") {
        return "---"; 
    }
    string uid = db.getUserIDByRoleID(teacherID);
    for (int i = 0; i < db.users.size(); i++) {
        if (db.users[i].getUserID() == uid) return db.users[i].getFullName();
    }
    return "Unknown";
}

string getSubjectName(Database& db, string classID) {
    for (int i = 0; i < db.classes.size(); i++) {
        if (db.classes[i].getClassID() == classID) return db.classes[i].getSubjectID();
    }
    return classID;
}

// Use Case
string studentSelectClass(Database& db) {
    cout << "\n";
    cout << "=========================================================================================\n";
    cout << "                               DANH SACH MON HOC (COURSE LIST)                           \n";
    cout << "=========================================================================================\n";
    
    if (db.classes.empty()) {
        cout << "   (Hien khong co mon hoc nao)\n";
        cout << "=========================================================================================\n";
        return "";
    }

    cout << left << setw(6) << "STT" 
         << left << setw(10) << "MA MH" 
         << left << setw(30) << "TEN MON HOC" 
         << left << setw(15) << "HOC KY"
         << left << setw(10) << "PHONG"
         << left << setw(20) << "GIANG VIEN" << endl;
    cout << "-----------------------------------------------------------------------------------------\n";

    for (int i = 0; i < db.classes.size(); i++) {
        string lecturer = findLecturerName(db, db.classes[i].getClassID());
        cout << left << setw(6) << i + 1 
             << left << setw(10) << db.classes[i].getClassID() 
             << left << setw(30) << db.classes[i].getSubjectID() 
             << left << setw(15) << "HK1-2024" 
             << left << setw(10) << db.classes[i].getRoom()
             << left << setw(20) << lecturer << endl;
    }
    cout << "=========================================================================================\n";
    cout << " [0]. Quay lai Dashboard\n";
    cout << ">> Nhap STT mon hoc muon thao tac: ";
    
    int choice;
    if (!(cin >> choice)) {
        cin.clear(); cin.ignore(1000, '\n'); return "";
    }

    if (choice > 0 && choice <= db.classes.size()) {
        return db.classes[choice - 1].getClassID();
    }
    return "";
}

Student::Student(string sid, string uid, string m) : studentID(sid), major(m) { userID=uid; }

void Student::showMenu(Database& db, User user) {
    int choice;
    do {
        cout << "\n============================================\n";
        cout << "      DASHBOARD SINH VIEN (" << studentID << ")\n";
        cout << "============================================\n";
        cout << "  [1]. VAO LOP HOC (Chon lop)\n  [2]. XEM THONG TIN CA NHAN\n  [0]. DANG XUAT\n>> Chon: "; cin >> choice;
        if (choice == 1) { string cid = promptSelectClass(db); if (cid != "") showClassMenu(db, cid); }
        else if (choice == 2) viewPersonalInfo(db, user, studentID);
    } while (choice != 0);
}

void Student::showClassMenu(Database& db, string classID) {
    int choice;
    do {
        cout << "\n--- LOP: " << classID << " ---\n  [1]. Diem danh\n  [2]. Lich su\n  [0]. Quay lai\n>> Chon: "; cin >> choice;
        switch(choice) { case 1: takeAttendance(db, classID); break; case 2: viewHistory(db, classID); break; }
    } while (choice != 0);
}

void Student::takeAttendance(Database& db, string classID) {
    cout << "\n--- CAC BUOI HOC DANG MO ---\n";
    cout << left << setw(15) << "SESSION ID" << setw(15) << "NGAY" << endl;
    cout << "------------------------------\n";
    bool found = false;
    for (int i = 0; i < db.sessions.size(); i++) 
        if (db.sessions[i].getIsOpen() && db.sessions[i].getClassID() == classID) {
            cout << left << setw(15) << db.sessions[i].getSessionID() << setw(15) << db.sessions[i].getDate() << endl; found = true;
        }
    if (!found) { cout << "(Trong)\n"; return; }

    string sID, pass; cout << "\n>> Nhap SessionID: "; cin >> sID;
    int idx = -1; for(int i=0; i<db.sessions.size(); i++) if(db.sessions[i].getSessionID() == sID) idx = i;
    if(idx == -1 || !db.sessions[idx].getIsOpen() || db.sessions[idx].getClassID() != classID) { cout << "Loi ID/Lop.\n"; return; }

    for(int i=0; i<db.attendances.size(); i++) if(db.attendances[i].getSessionID() == sID && db.attendances[i].getStudentID() == studentID) { cout << "Da diem danh roi.\n"; return; }

    cout << ">> Mat khau: "; cin >> pass;
    if (pass == db.sessions[idx].getPassword()) {
        db.attendances.push_back(Attendance(studentID, sID, "Co Mat", getCurrentTime())); db.saveAttendance(); cout << "Thanh cong!\n";
    } else cout << "Sai mat khau.\n";
}

void Student::viewHistory(Database& db, string classID) {
    cout << "\n--- LICH SU ---\n";
    cout << left << setw(15) << "SESSION" << setw(15) << "TRANG THAI" << setw(15) << "GIO" << endl;
    for (int i = 0; i < db.attendances.size(); i++) {
        if (db.attendances[i].getStudentID() == studentID) {
            string sClass = "";
            for(int j=0; j<db.sessions.size(); j++) if(db.sessions[j].getSessionID() == db.attendances[i].getSessionID()) sClass = db.sessions[j].getClassID();
            if (sClass == classID) cout << left << setw(15) << db.attendances[i].getSessionID() << setw(15) << db.attendances[i].getStatus() << setw(15) << db.attendances[i].getCheckinTime() << endl;
        }
    }
    cout << "Nhan Enter..."; string d; getline(cin, d); getline(cin, d);
}