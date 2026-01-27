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

