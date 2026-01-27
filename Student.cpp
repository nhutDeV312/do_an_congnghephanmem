#include "Student.h"
#include "Database.h"
#include <iostream>
#include <iomanip>

using namespace std;

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