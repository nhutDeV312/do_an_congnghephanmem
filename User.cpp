#include "User.h"
#include "Database.h"
#include "Student.h"
#include "Teacher.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <limits> // Thu vien de xoa bo dem

using namespace std;

User::User(string uid, string uname, string pass, string fname, string gen, string ph, string r) {
    userID=uid; username=uname; password=pass; fullName=fname; gender=gen; phone=ph; role=r;
}

// --- CÁC HÀM GIAO DIỆN CHUNG ---

string User::getCurrentTime() {
    time_t now = time(0); tm *ltm = localtime(&now); char b[80]; strftime(b,80,"%H:%M",ltm); return string(b);
}

string User::promptSelectClass(Database& db) {
    cout << "\n";
    cout << "===============================================================\n";
    cout << "                  DANH SACH LOP HOC (CLASSES)                  \n";
    cout << "===============================================================\n";
    if (db.classes.empty()) { cout << "   (Trong)\n"; return ""; }
    
    cout << left << setw(6) << " STT" << left << setw(12) << "MA LOP" << left << setw(30) << "TEN MON HOC" << left << setw(10) << "PHONG" << endl;
    cout << "---------------------------------------------------------------\n";
    for (int i = 0; i < db.classes.size(); i++) {
        cout << "  " << left << setw(4) << i + 1 << left << setw(12) << db.classes[i].getClassID() 
             << left << setw(30) << db.classes[i].getSubjectID() << left << setw(10) << db.classes[i].getRoom() << endl;
    }
    cout << "===============================================================\n";
    cout << " [0]. Quay lai\n>> Nhap STT lop: ";
    
    int choice; 
    // --- FIX LOI NHAP LIEU ---
    if (!(cin >> choice)) {
        cin.clear(); cin.ignore(1000, '\n'); return "";
    }
    
    if (choice > 0 && choice <= db.classes.size()) return db.classes[choice - 1].getClassID();
    return "";
}

void User::viewPersonalInfo(Database& db, User user, string roleID) {
    cout << "\n***************************************************************\n";
    cout << "* THONG TIN CA NHAN                         *\n";
    cout << "***************************************************************\n";
    cout << left << setw(20) << " Ho va Ten:" << user.getFullName() << endl;
    cout << left << setw(20) << " So dien thoai:" << user.getPhone() << endl;
    
    if(user.getRole() == "teacher") {
        for(int i = 0; i < db.teachers.size(); i++) 
            if(db.teachers[i].getTeacherID() == roleID) {
                cout << left << setw(20) << " Ma Giang Vien:" << roleID << endl;
                cout << left << setw(20) << " Khoa:" << db.teachers[i].getDepartment() << endl;
            }
    } else {
        for(int i = 0; i < db.students.size(); i++) 
            if(db.students[i].getStudentID() == roleID) {
                cout << left << setw(20) << " MSSV:" << roleID << endl;
                cout << left << setw(20) << " Nganh hoc:" << db.students[i].getMajor() << endl;
            }
    }
    cout << "***************************************************************\n";
    cout << "Nhan Enter..."; 
    cin.ignore(); string d; getline(cin, d);
}

// --- LOGIC HỆ THỐNG ---

void User::run(Database& db) {
    db.loadAllData();
    User currentUser; string currentRoleID;
    
    while(true) {
        cout << "\n============================================\n";
        cout << "       HE THONG DIEM DANH (ATTENDANCE)      \n";
        cout << "============================================\n";
        cout << "  [1]. DANG NHAP HE THONG\n  [2]. QUEN MAT KHAU\n  [0]. THOAT\n";
        cout << "============================================\n";
        cout << ">> Lua chon cua ban: "; 
        
        int c; 
        cin >> c;

        // --- CODE FIX LỖI LẶP VÔ TẬN ---
        if (cin.fail()) {
            if (cin.eof()) {
                cout << "\n[System] Mat ket noi dau vao. Dang thoat...\n";
                break; 
            }
            cin.clear(); 
            cin.ignore(1000, '\n');
            cout << ">>> LOI: Vui long chi nhap so!\n";
            continue; 
        }
        // -------------------------------
        
        if(c==1) {
            if(login(db, currentUser, currentRoleID)) {
                cout << "\n>>> DANG NHAP THANH CONG! Xin chao " << currentUser.getFullName() << "\n";
                
                // --- FIX LOI WINDOWS/LINUX (CRLF) ---
                // Xoa ky tu \r o cuoi chuoi role neu co (do doc file Windows tren Linux)
                string role = currentUser.getRole();
                if (!role.empty() && role.back() == '\r') role.pop_back();
                
                bool foundMenu = false; // Kiem tra xem co vao duoc menu khong

                if(role == "student") {
                    for(int i=0;i<db.students.size();i++) 
                        if(db.students[i].getStudentID()==currentRoleID) {
                            db.students[i].showMenu(db, currentUser);
                            foundMenu = true;
                        }
                } else if(role == "teacher") {
                    for(int i=0;i<db.teachers.size();i++) 
                        if(db.teachers[i].getTeacherID()==currentRoleID) {
                            db.teachers[i].showMenu(db, currentUser);
                            foundMenu = true;
                        }
                }

                // Neu dang nhap thanh cong ma khong hien menu -> Bao loi data
                if (!foundMenu) {
                    cout << ">>> CANH BAO: Khong tim thay du lieu chi tiet cho tai khoan nay!\n";
                    cout << "    Role he thong nhan duoc: '" << role << "'\n";
                    cout << "    ID he thong tim duoc: '" << currentRoleID << "'\n";
                    cout << "    Vui long kiem tra lai file students.txt hoac teachers.txt.\n";
                }

            } else cout << "\n>>> LOI: Sai Ten dang nhap hoac Mat khau!\n";
        } else if(c==2) forgotPassword(db);
        else if(c==0) {
            cout << "\nTam biet!\n";
            break;
        }
    }
}

bool User::login(Database& db, User& curUser, string& curRID) {
    string u,p; 
    cout << "\n--- DANG NHAP ---\n";
    cout << "Username: "; cin >> u; 
    cout << "Password: "; cin >> p;
    
    for(int i=0; i<db.users.size(); i++) {
        if(db.users[i].getUsername()==u && db.users[i].checkPassword(p)) {
            curUser = db.users[i];
            
            // --- FIX LOI ROLE ---
            string role = curUser.getRole();
            if (!role.empty() && role.back() == '\r') role.pop_back();

            if (role == "student") 
                curRID = db.getStudentIDByUserID(curUser.getUserID());
            else if (role == "teacher") 
                curRID = db.getTeacherIDByUserID(curUser.getUserID());
            
            return true;
        }
    }
    return false;
}

void User::forgotPassword(Database& db) {
    cout << "\n--- KHOI PHUC MAT KHAU ---\n";
    string id, ph; 
    cout << "Nhap ID (Username/MSSV/MaGV): "; cin >> id; 
    cout << "Nhap So Dien Thoai da dang ky : "; cin >> ph;
    
    string uid = db.getUserIDByRoleID(id);
    if(uid=="") for(int i=0; i<db.users.size(); i++) if(db.users[i].getUserID()==id || db.users[i].getUsername()==id) uid=db.users[i].getUserID();
    
    if(uid=="") { cout << ">>> LOI: Khong tim thay Tai khoan/ID nay.\n"; return; }
    
    bool found = false;
    for(int i=0; i<db.users.size(); i++) {
        if(db.users[i].getUserID()==uid) {
            if(db.users[i].getPhone()==ph) {
                found = true;
                string np; cout << ">> Xin chao " << db.users[i].getFullName() << ". Nhap Mat khau moi: "; cin >> np;
                db.users[i].setPassword(np); db.saveUsers(); 
                cout << ">>> DOI MAT KHAU THANH CONG!\n";
            } else cout << ">>> LOI: So dien thoai khong khop!\n";
            break;
        }
    }
}