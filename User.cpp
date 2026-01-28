#include "User.h"
#include "Database.h"
#include "Student.h"
#include "Teacher.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <limits> 

using namespace std;

//Update Constructor: Them dob
User::User(string uid, string uname, string pass, string fname, string d, string gen, string ph, string r) {
    userID=uid; username=uname; password=pass; fullName=fname; dob=d; gender=gen; phone=ph; role=r;
}

string User::getCurrentTime() {
    time_t now = time(0); tm *ltm = localtime(&now); char b[80]; strftime(b,80,"%H:%M",ltm); return string(b);
}

string User::promptSelectClass(Database& db) {
    cout << "\n===============================================================\n";
    cout << "                  DANH SACH LOP HOC (CLASSES)                  \n";
    cout << "===============================================================\n";
    if (db.classes.empty()) 
    { cout << "   (Trong)\n"; return ""; }
    cout << left << setw(6) << " STT" << left << setw(12) << "MA LOP" << left << setw(30) << "TEN MON HOC" << left << setw(10) << "PHONG" << endl;
    cout << "---------------------------------------------------------------\n";
    for (int i = 0; i < db.classes.size(); i++) {
        cout << "  " << left << setw(4) << i + 1 << left << setw(12) << db.classes[i].getClassID() 
             << left << setw(30) << db.classes[i].getSubjectID() << left << setw(10) << db.classes[i].getRoom() << endl;
    }
    cout << "===============================================================\n";
    cout << " [0]. Quay lai\n>> Nhap STT lop: ";
    
    int choice; 
    if (!(cin >> choice)) { cin.clear(); cin.ignore(1000, '\n'); return ""; }
    if (choice > 0 && choice <= db.classes.size()) return db.classes[choice - 1].getClassID();
    return "";
}

//View Personal Info (Updated Loop)
void User::viewPersonalInfo(Database& db, User user, string roleID) {
    cout << "\n";
    cout << "========================================================================\n";
    cout << "                       THONG TIN CA NHAN (PROFILE)                      \n";
    cout << "========================================================================\n";
    
    string genderStr = (user.getGender() == "M" ? "Nam" : "Nu");
    
    if (user.getRole() == "teacher") {
        //GIANG VIEN: ID, Name, DOB, Gender, Phone, Degree
        string degree = "---";
        //Su dung vong lap for chi so don gian thay vi auto
        for (int i = 0; i < db.teachers.size(); i++) {
            if (db.teachers[i].getTeacherID() == roleID) {
                degree = db.teachers[i].getDegree();
                break;
            }
        }

        cout << left << setw(30) << " Ma Giang Vien (ID):" << roleID << endl;
        cout << left << setw(30) << " Ho va Ten (Full Name):" << user.getFullName() << endl;
        cout << left << setw(30) << " Ngay sinh (Date of Birth):" << user.getDob() << endl;
        cout << left << setw(30) << " Gioi tinh (Gender):" << genderStr << endl;
        cout << left << setw(30) << " So dien thoai (Phone):" << user.getPhone() << endl;
        cout << left << setw(30) << " Hoc vi (Degree):" << degree << endl;

    } else {
        //SINH VIEN: ID, Name, DOB, Gender, Major, Phone
        string major = "---";
        for (int i = 0; i < db.students.size(); i++) {
            if (db.students[i].getStudentID() == roleID) {
                major = db.students[i].getMajor();
                break;
            }
        }

        cout << left << setw(30) << " MSSV (Student ID):" << roleID << endl;
        cout << left << setw(30) << " Ho va Ten (Full Name):" << user.getFullName() << endl;
        cout << left << setw(30) << " Ngay sinh (Date of Birth):" << user.getDob() << endl;
        cout << left << setw(30) << " Gioi tinh (Gender):" << genderStr << endl;
        cout << left << setw(30) << " Chuyen nganh (Major):" << major << endl;
        cout << left << setw(30) << " So dien thoai (Phone):" << user.getPhone() << endl;
    }
    
    cout << "========================================================================\n";
    cout << "Nhan Enter de quay lai...";
    cin.ignore(1000, '\n'); string d; getline(cin, d);
}

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
        
        int c; cin >> c;
        if (cin.fail()) {
            if (cin.eof()) break; 
            cin.clear(); cin.ignore(1000, '\n');
            cout << ">>> LOI: Vui long chi nhap so!\n"; continue; 
        }
        
        if(c==1) {
            if(login(db, currentUser, currentRoleID)) {
                cout << "\n>>> DANG NHAP THANH CONG! Xin chao " << currentUser.getFullName() << "\n";
                string role = currentUser.getRole();
                if (!role.empty() && role.back() == '\r') role.pop_back();
                
                if(role == "student") {
                    for(int i=0;i<db.students.size();i++) 
                        if(db.students[i].getStudentID()==currentRoleID) db.students[i].showMenu(db, currentUser);
                } else if(role == "teacher") {
                    for(int i=0;i<db.teachers.size();i++) 
                        if(db.teachers[i].getTeacherID()==currentRoleID) db.teachers[i].showMenu(db, currentUser);
                }
            } else cout << "\n>>> LOI: Sai Ten dang nhap hoac Mat khau!\n";
        } else if(c==2) forgotPassword(db);
        else if(c==0) break;
    }
}

bool User::login(Database& db, User& curUser, string& curRID) {
    string u,p; cout << "\n--- DANG NHAP ---\nUsername: "; cin >> u; cout << "Password: "; cin >> p;
    for(int i=0; i<db.users.size(); i++) {
        if(db.users[i].getUsername()==u && db.users[i].checkPassword(p)) {
            curUser = db.users[i];
            string role = curUser.getRole();
            if (!role.empty() && role.back() == '\r') role.pop_back();
            if (role == "student") curRID = db.getStudentIDByUserID(curUser.getUserID());
            else if (role == "teacher") curRID = db.getTeacherIDByUserID(curUser.getUserID());
            return true;
        }
    }
    return false;
}

void User::forgotPassword(Database& db) {
    cout << "\n--- KHOI PHUC MAT KHAU ---\n";
    cout << "Nhap ID (MSSV/MaGV): "; string id; cin >> id; 
    cout << "Nhap So Dien Thoai da dang ky : "; string ph; cin >> ph;
    
    string uid = db.getUserIDByRoleID(id);
    if(uid=="") for(int i=0; i<db.users.size(); i++) if(db.users[i].getUserID()==id || db.users[i].getUsername()==id) uid=db.users[i].getUserID();
    
    if(uid=="") { cout << ">>> LOI: Khong tim thay Tai khoan/ID nay.\n"; return; }
    
    for(int i=0; i<db.users.size(); i++) {
        if(db.users[i].getUserID()==uid) {
            if(db.users[i].getPhone()==ph) {
                string np; cout << ">> Xin chao " << db.users[i].getFullName() << ". Nhap Mat khau moi: "; cin >> np;
                db.users[i].setPassword(np); db.saveUsers(); 
                cout << ">>> DOI MAT KHAU THANH CONG!\n";
            } else cout << ">>> LOI: So dien thoai khong khop!\n";
            return;
        }
    }
}