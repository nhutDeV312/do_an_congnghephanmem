
#include "Student.h"
#include "Database.h"
#include "Teacher.h" 
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;

//Chuc nang ho tro

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

// Chon class
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
        cout << "\n";
        cout << "============================================\n";
        cout << "      DASHBOARD SINH VIEN (" << user.getFullName() << ")\n";
        cout << "============================================\n";
        cout << "  [1]. XEM DANH SACH MON HOC (View Courses)\n";
        cout << "  [2]. XEM THONG TIN CA NHAN (Profile)\n";
        cout << "  [0]. DANG XUAT (Logout)\n";
        cout << "============================================\n";
        cout << ">> Nhap lua chon: "; 
        cin >> choice;
        
        if (choice == 1) {
            string classID = studentSelectClass(db);
            if (classID != "") showClassMenu(db, classID);
        } else if (choice == 2) {
            viewPersonalInfo(db, user, studentID);
        }
    } while (choice != 0);
}

void Student::showClassMenu(Database& db, string classID) {
    int choice;
    do {
        cout << "\n";
        cout << "=== THAO TAC MON HOC: " << classID << " ===\n";
        cout << "  [1]. DIEM DANH (Take Attendance)\n";
        cout << "  [2]. XEM LICH SU (View History)\n";
        cout << "  [0]. Quay lai\n";
        cout << ">> Lua chon: "; cin >> choice;
        switch(choice) {
            case 1: takeAttendance(db, classID); break;
            case 2: viewHistory(db, classID); break;
        }
    } while (choice != 0);
}

//  Diem danh
void Student::takeAttendance(Database& db, string classID) {
    cout << "\n--- CAC BUOI HOC DANG MO ---\n";
    cout << left << setw(15) << "SESSION ID" << setw(15) << "NGAY" << setw(20) << "TRANG THAI" << endl;
    cout << "--------------------------------------------------\n";
    
    bool found = false;
    for (int i = 0; i < db.sessions.size(); i++) {
        if (db.sessions[i].getIsOpen() && db.sessions[i].getClassID() == classID) {
            cout << left << setw(15) << db.sessions[i].getSessionID() 
                 << setw(15) << db.sessions[i].getDate()
                 << setw(20) << "DANG MO (OPEN)" << endl;
            found = true;
        }
    }
    if (!found) { cout << "   (Hien khong co buoi hoc nao dang mo)\n"; return; }

    string sID, pass; 
    cout << "\n>> Nhap SessionID de diem danh: "; cin >> sID;
    
    int idx = -1;
    for(int i=0; i<db.sessions.size(); i++) if(db.sessions[i].getSessionID() == sID) idx = i;
    
    if(idx == -1) { cout << ">>> LOI: Khong tim thay SessionID nay.\n"; return; }

    Session s = db.sessions[idx];
    if(s.getClassID() != classID) { cout << ">>> LOI: Session nay khong thuoc lop ban dang chon.\n"; return; }
    if(!s.getIsOpen()) { cout << ">>> LOI: Buoi hoc da DONG, khong the diem danh.\n"; return; }

    // Hien thi chi tiet phien
    string lecturerName = findLecturerName(db, classID);
    string subjectName = getSubjectName(db, classID);
    cout << "\n--- THONG TIN BUOI DIEM DANH ---\n";
    cout << " Mon hoc    : " << subjectName << "\n";
    cout << " Giang vien : " << lecturerName << "\n";
    cout << " Thoi gian  : " << s.getDate() << " (" << s.getStartTime() << " - " << s.getEndTime() << ")\n";
    cout << "--------------------------------\n";

    for(int i=0; i<db.attendances.size(); i++) 
        if(db.attendances[i].getSessionID() == sID && db.attendances[i].getStudentID() == studentID) {
            cout << ">>> BAN DA DIEM DANH ROI! (Trang thai: " << db.attendances[i].getStatus() << ")\n"; return; 
        }

    cout << ">> Nhap Mat khau (" << s.getPassword().length() << " ky tu): "; cin >> pass;
    
    if (pass == s.getPassword()) {
        db.attendances.push_back(Attendance(studentID, sID, "Co Mat", getCurrentTime()));
        db.saveAttendance();
        cout << "\n>>> DIEM DANH THANH CONG! (Attendance Successful)\n";
        cout << ">>> Thoi gian ghi nhan: " << getCurrentTime() << "\n";
    } else cout << "\n>>> SAI MAT KHAU! Vui long thu lai.\n";
}

void Student::viewHistory(Database& db, string classID) {
    cout << "\n";
    cout << "=============================================\n";
    cout << "       LICH SU DIEM DANH (HISTORY)           \n";
    cout << "=============================================\n";
    cout << left << setw(15) << "SESSION ID" << setw(15) << "TRANG THAI" << setw(15) << "GIO CHECK-IN" << endl;
    cout << "---------------------------------------------\n";
    
    int totalSessions = 0;
    int presentCount = 0;
    int absentCount = 0;

    // Dem tong so buoi hoc cua lop 
    for(int i=0; i<db.sessions.size(); i++) {
        if(db.sessions[i].getClassID() == classID) {
            totalSessions++;
            // Kiem tra trang thai cua sinh vien trong buoi hoc
            bool isPresent = false;
            string status = "Vang"; // Mac dinh la vang
            string time = "---";

            for(int j=0; j<db.attendances.size(); j++) {
                if(db.attendances[j].getSessionID() == db.sessions[i].getSessionID() && 
                   db.attendances[j].getStudentID() == studentID) {
                    status = db.attendances[j].getStatus();
                    time = db.attendances[j].getCheckinTime();
                    isPresent = true;
                    break;
                }
            }

            if(isPresent) presentCount++;
            else absentCount++;

            cout << left << setw(15) << db.sessions[i].getSessionID() 
                 << setw(15) << status 
                 << setw(15) << time << endl;
        }
    }
    
    if (totalSessions == 0) {
        cout << "   (Chua co du lieu buoi hoc nao)\n";
    } else {
        cout << "---------------------------------------------\n";
        cout << " TONG KET:\n";
        cout << " - Tong so buoi : " << totalSessions << "\n";
        cout << " - Co mat       : " << presentCount << "\n";
        cout << " - Vang         : " << absentCount << "\n";
        
        float rate = (float)presentCount / totalSessions * 100;
        cout << " => TY LE DIEM DANH: " << fixed << setprecision(1) << rate << "%\n";
    }
    cout << "=============================================\n";
    
    cout << "Nhan Enter de tiep tuc..."; 
    cin.ignore(1000, '\n'); string d; getline(cin, d);
}