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