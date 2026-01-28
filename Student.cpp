#include "Student.h"
#include "Database.h"
#include "Teacher.h" 
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;

// --- Helper Functions (Local) ---

// Ham tim ten giang vien (Dua vao session da tao)
string findLecturerName(Database& db, string classID) {
    string teacherID = "";
    // Tim xem ai da tao session cho lop nay
    for (int i = 0; i < db.sessions.size(); i++) {
        if (db.sessions[i].getClassID() == classID) {
            teacherID = db.sessions[i].getTeacherID();
            break;
        }
    }
    
    if (teacherID == "") return "---"; // Chua co session nao -> Chua biet GV

    string uid = db.getUserIDByRoleID(teacherID);
    for (int i = 0; i < db.users.size(); i++) {
        if (db.users[i].getUserID() == uid) return db.users[i].getFullName();
    }
    return "Unknown";
}

// 8. Use Case: View Course Info (DA CHINH SUA GIAO DIEN)
string studentSelectClass(Database& db) {
    cout << "\n";
    cout << "====================================================================================================\n";
    cout << "                                   DANH SACH MON HOC (COURSE LIST)                                  \n";
    cout << "====================================================================================================\n";
    
    if (db.classes.empty()) {
        cout << "   (Hien khong co mon hoc nao)\n";
        cout << "====================================================================================================\n";
        return "";
    }

    // Header bang: Da bo cot GIANG VIEN va doi MA MH thanh MA LOP
    cout << left << setw(6)  << "STT" 
         << left << setw(15) << "MA LOP" 
         << left << setw(35) << "TEN MON HOC"      
         << left << setw(15) << "HOC KY"
         << left << setw(10) << "PHONG" << endl;
    cout << "----------------------------------------------------------------------------------------------------\n";

    for (int i = 0; i < db.classes.size(); i++) {
        // string lecturer = findLecturerName(db, db.classes[i].getClassID()); // Khong can hien thi giang vien nua
        
        cout << left << setw(6)  << i + 1 
             << left << setw(15) << db.classes[i].getClassID() 
             << left << setw(35) << db.classes[i].getSubjectID() 
             << left << setw(15) << "HK1-2024" // Placeholder
             << left << setw(10) << db.classes[i].getRoom() << endl;
    }
    cout << "====================================================================================================\n";
    cout << " [0]. Quay lai Dashboard\n";
    cout << ">> Nhap STT mon hoc muon thao tac: ";
    
    int choice;
    // Fix loi nhap lieu
    if (!(cin >> choice)) {
        cin.clear(); cin.ignore(1000, '\n'); return "";
    }

    if (choice > 0 && choice <= db.classes.size()) {
        return db.classes[choice - 1].getClassID();
    }
    return "";
}

// --- Implementation ---

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

    // Hien thi chi tiet buoi hoc
    string lecturerName = findLecturerName(db, classID);
    cout << "\n--- THONG TIN BUOI DIEM DANH ---\n";
    cout << " Mon hoc    : " << classID << "\n";
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
        cout << "\n>>> DIEM DANH THANH CONG! (Time: " << getCurrentTime() << ")\n";
    } else cout << "\n>>> SAI MAT KHAU!\n";
}

void Student::viewHistory(Database& db, string classID) {
    cout << "\n";
    cout << "====================================================================\n";
    cout << "                    LICH SU DIEM DANH (HISTORY)                     \n";
    cout << "====================================================================\n";
    cout << left << setw(15) << "NGAY" << setw(15) << "GIO HOC" << setw(15) << "TRANG THAI" << setw(15) << "GIO CHECK-IN" << endl;
    cout << "--------------------------------------------------------------------\n";
    
    int totalSessions = 0;
    int presentCount = 0;
    int absentCount = 0;

    for(int i=0; i<db.sessions.size(); i++) {
        if(db.sessions[i].getClassID() == classID) {
            totalSessions++;
            bool isPresent = false;
            string status = "Vang"; 
            string timeCheckin = "---";

            for(int j=0; j<db.attendances.size(); j++) {
                if(db.attendances[j].getSessionID() == db.sessions[i].getSessionID() && 
                   db.attendances[j].getStudentID() == studentID) {
                    status = db.attendances[j].getStatus();
                    timeCheckin = db.attendances[j].getCheckinTime();
                    isPresent = true;
                    break;
                }
            }

            if(isPresent) presentCount++;
            else absentCount++;

            string sessionTime = db.sessions[i].getStartTime() + "-" + db.sessions[i].getEndTime();
            
            cout << left << setw(15) << db.sessions[i].getDate()
                 << setw(15) << sessionTime
                 << setw(15) << status 
                 << setw(15) << timeCheckin << endl;
        }
    }
    
    if (totalSessions == 0) {
        cout << "   (Chua co du lieu buoi hoc nao)\n";
    } else {
        cout << "--------------------------------------------------------------------\n";
        cout << " TONG KET:\n";
        cout << " - Tong so buoi : " << totalSessions << "\n";
        cout << " - Co mat       : " << presentCount << "\n";
        cout << " - Vang         : " << absentCount << "\n"; 
    }
    cout << "====================================================================\n";
    
    cout << "Nhan Enter de tiep tuc..."; 
    cin.ignore(1000, '\n'); string d; getline(cin, d);
}