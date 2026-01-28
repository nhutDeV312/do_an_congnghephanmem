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

// --- GIAO DIEN TAO BUOI HOC MOI ---
void Teacher::createSession(Database& db, string classID) {
    string d, s, e, p;
    cout << "\n-------------------------------------------------------\n";
    cout << "   TAO BUOI DIEM DANH MOI (Lop " << classID << ")\n";
    cout << "-------------------------------------------------------\n";
    
    cout << "  Buoc 1: Nhap Ngay hoc (dd/mm/yyyy) : "; cin >> d;
    cout << "  Buoc 2: Nhap Gio bat dau (HH:MM)   : "; cin >> s;
    cout << "  Buoc 3: Nhap Gio ket thuc (HH:MM)  : "; cin >> e;
    
    if (e <= s) { 
        cout << "\n>>> LOI: Gio ket thuc phai lon hon gio bat dau!\n"; 
        return; 
    }

    for(int i=0; i<db.sessions.size(); i++) {
        if(db.sessions[i].getClassID() == classID && db.sessions[i].getDate() == d && db.sessions[i].getStartTime() == s) {
            cout << "\n>>> LOI: Buoi hoc nay da ton tai roi (Trung lich)!\n"; return;
        }
    }

    cout << "  Buoc 4: Dat Mat khau diem danh     : "; cin >> p;
    
    string sid = "SES" + to_string(time(0));
    db.sessions.push_back(Session(sid, classID, teacherID, d, s, e, true, p));
    db.saveSessions();
    
    cout << "\n>>> THANH CONG! Ma session: " << sid << " | Trang thai: DANG MO\n";
}

// --- GIAO DIEN SUA BUOI HOC  ---
void Teacher::editSession(Database& db, string classID) {
    cout << "\n--- DANH SACH CAC BUOI DIEM DANH ---\n";
    cout << left << setw(6) << "STT" << setw(15) << "NGAY" << setw(15) << "THOI GIAN" << setw(12) << "TRANG THAI" << endl;
    cout << "-------------------------------------------------------\n";
    
    vector<int> idxs;
    for(int i=0; i<db.sessions.size(); i++) {
        if(db.sessions[i].getClassID() == classID && db.sessions[i].getTeacherID() == teacherID) {
            cout << left << setw(6) << idxs.size()+1 
                 << setw(15) << db.sessions[i].getDate() 
                 << setw(15) << (db.sessions[i].getStartTime() + "-" + db.sessions[i].getEndTime())
                 << setw(12) << db.sessions[i].getStatusString() << endl;
            idxs.push_back(i);
        }
    }

    if(idxs.empty()) { cout << "   (Chua co buoi hoc nao)\n"; return; }
    
    int c; 
    cout << "\n>> Chon STT buoi muon sua (0 de quay lai): "; cin >> c;
    if(c<=0 || c>idxs.size()) return;
    
    int realIdx = idxs[c-1];
    Session s = db.sessions[realIdx];

    vector<Attendance> bufAtt;
    for(int i=0; i<db.attendances.size(); i++) 
        if(db.attendances[i].getSessionID() == s.getSessionID()) bufAtt.push_back(db.attendances[i]);

    while(true) {
        cout << "\n============================================\n";
        cout << "   DANG CHINH SUA: " << s.getDate() << "\n";
        cout << "============================================\n";
        cout << "  [1]. Sua thong tin co ban (Ngay, Gio, Mat khau)\n";
        cout << "  [2]. Sua trang thai diem danh SV (Vang -> Co mat)\n";
        cout << "  [3]. LUU THAY DOI (Save)\n";
        cout << "  [0]. HUY BO (Cancel)\n";
        cout << "============================================\n";
        cout << ">> Lua chon: "; 
        int act; cin >> act;

        if(act==1) {
            cout << "\n--- SUA THONG TIN CHUNG ---\n";
            string d, st, en, pa; 
            
            cout << "  - Ngay cu: " << s.getDate() << "  -> Nhap Ngay moi: "; cin >> d;
            cout << "  - Gio BD cu: " << s.getStartTime() << " -> Nhap Gio BD moi: "; cin >> st;
            cout << "  - Gio KT cu: " << s.getEndTime() << "   -> Nhap Gio KT moi: "; cin >> en;
            
            if (en <= st) {
                cout << "\n>>> LOI: Gio ket thuc phai lon hon gio bat dau! (Chua luu)\n";
            } else {
                cout << "  - Pass cu: " << s.getPassword() << "   -> Nhap Pass moi: "; cin >> pa;
                s.setDate(d); s.setStartTime(st); s.setEndTime(en); s.setPassword(pa);
                cout << "  - Trang thai hien tai: " << s.getStatusString() << "\n";
                cout << "  - Chon trang thai moi (1: MO, 0: DONG): "; int op; cin >> op; 
                s.setIsOpen(op==1);
                cout << ">>> Da cap nhat thong tin tren bo nho tam.\n";
            }
        } 
        else if(act==2) {
            cout << "\n--- CAP NHAT TRANG THAI SINH VIEN ---\n";
            
            // --- KIEM TRA DANH SACH RONG ---
            if (bufAtt.empty()) {
                cout << "Danh sach sinh vien hien tai: (Trong)\n";
                cout << "------------------------------\n";
                cout << ">>> THONG BAO: Chua co sinh vien nao diem danh trong buoi nay!\n";
                cout << "    Ban khong the sua trang thai khi danh sach rong.\n";
                cout << "    Vui long doi sinh vien diem danh hoac quay lai sau.\n";
                
            } 
            else {
                // Neu co du lieu thi hien thi va cho sua
                cout << "Danh sach sinh vien trong buoi nay:\n";
                cout << left << setw(15) << "MSSV" << setw(15) << "TRANG THAI" << endl;
                cout << "------------------------------\n";
                for (int i = 0; i < bufAtt.size(); i++) {
                    cout << left << setw(15) << bufAtt[i].getStudentID() 
                         << setw(15) << bufAtt[i].getStatus() << endl;
                }
                cout << "------------------------------\n";

                string sid; int stat; 
                cout << "  - Nhap MSSV can sua: "; cin >> sid; 
                cout << "  - Chon trang thai (1.CoMat | 2.Vang | 3.Tre): "; cin >> stat;
                string ss = (stat==1?"Co Mat":(stat==2?"Vang":"Tre"));
                
                bool f=false;
                for(int i=0; i<bufAtt.size(); i++) {
                    if(bufAtt[i].getStudentID()==sid) { 
                        bufAtt[i].setStatus(ss); f=true; 
                        cout << ">>> Da cap nhat SV " << sid << " thanh: " << ss << endl;
                    }
                }
                if(!f) { 
                    bufAtt.push_back(Attendance(sid, s.getSessionID(), ss, "Manual")); 
                    cout << ">>> Da them moi SV " << sid << " vao danh sach.\n";
                }
            }
        } 
        else if(act==3) {
            db.sessions[realIdx] = s;
            vector<Attendance> newAtt;
            for(int i=0; i<db.attendances.size(); i++) 
                if(db.attendances[i].getSessionID() != s.getSessionID()) newAtt.push_back(db.attendances[i]);
            for(int i=0; i<bufAtt.size(); i++) newAtt.push_back(bufAtt[i]);
            
            db.attendances = newAtt;
            db.saveSessions(); db.saveAttendance();
            cout << "\n>>> DA LUU THANH CONG VAO HE THONG!\n"; break;
        } 
        else if(act==0) break;
    }
}

void Teacher::viewReport(Database& db, string classID) {
    cout << "\n--- CHON BUOI DE XEM BAO CAO ---\n";
    vector<int> idxs;
    cout << left << setw(6) << "STT" << setw(15) << "NGAY" << endl;
    cout << "-----------------------\n";
    for(int i=0; i<db.sessions.size(); i++) {
        if(db.sessions[i].getClassID() == classID && db.sessions[i].getTeacherID() == teacherID) {
            cout << left << setw(6) << idxs.size()+1 << setw(15) << db.sessions[i].getDate() << endl;
            idxs.push_back(i);
        }
    }
    if(idxs.empty()) { cout << "(Trong)\n"; return; }
    
    int c; cout << "\n>> Chon so (0 de quay lai): "; cin >> c;
    if(c<=0 || c>idxs.size()) return;
    
    string sid = db.sessions[idxs[c-1]].getSessionID();

    cout << "\n============================================\n";
    cout << "   KET QUA DIEM DANH (" << db.sessions[idxs[c-1]].getDate() << ")\n";
    cout << "============================================\n";
    cout << left << setw(15) << "MSSV" << setw(15) << "TRANG THAI" << endl;
    cout << "------------------------------\n";
    
    int cnt=0;
    for(int i=0; i<db.attendances.size(); i++) {
        if(db.attendances[i].getSessionID() == sid) {
            cout << left << setw(15) << db.attendances[i].getStudentID() 
                 << setw(15) << db.attendances[i].getStatus() << endl;
            cnt++;
        }
    }
    cout << "------------------------------\n";
    cout << "Tong so sinh vien da ghi nhan: " << cnt << endl;
    string d; getline(cin, d); getline(cin, d);
}