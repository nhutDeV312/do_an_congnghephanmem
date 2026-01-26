#include "Teacher.h"
#include "Database.h"
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

Teacher::Teacher(string tid, string uid, string dept, string deg) : teacherID(tid), department(dept), degree(deg) { userID=uid; }

void Teacher::showMenu(Database& db, User user) {
    int choice;
    do {
        cout << "\n";
        cout << "=======================================================\n";
        cout << "      DASHBOARD GIANG VIEN (" << user.getFullName() << ")\n";
        cout << "=======================================================\n";
        cout << "  [1]. QUAN LY LOP HOC (Vao lop)\n";
        cout << "  [2]. XEM THONG TIN CA NHAN\n";
        cout << "  [0]. DANG XUAT\n";
        cout << "=======================================================\n";
        cout << ">> Nhap lua chon: "; 
        cin >> choice;

        if (choice == 1) {
            string classID = promptSelectClass(db);
            if (classID != "") showClassMenu(db, classID);
        } else if (choice == 2) {
            viewPersonalInfo(db, user, teacherID);
        }
    } while (choice != 0);
}

void Teacher::showClassMenu(Database& db, string classID) {
    int choice;
    do {
        cout << "\n";
        cout << "=== QUAN LY LOP: " << classID << " ===\n";
        cout << "  [1]. TAO buoi diem danh moi\n";
        cout << "  [2]. SUA / DONG MO diem danh\n";
        cout << "  [3]. XEM BAO CAO diem danh\n";
        cout << "  [0]. Quay lai Dashboard\n";
        cout << ">> Lua chon cua ban: "; 
        cin >> choice;

        switch(choice) {
            case 1: createSession(db, classID); break;
            case 2: editSession(db, classID); break;
            case 3: viewReport(db, classID); break;
        }
    } while (choice != 0);
}