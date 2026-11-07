#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include "ConsoleUI.cpp"
using namespace std;

class Ngay{
    int d, m, y;
public:
    int getD() const {return d;}
    void setD(int x) {d = x;}
    int getM() const {return m;}
    void setM(int x) {m = x;}
    int getY() const {return y;}
    void setY(int x) {y = x;}

    Ngay(int _d = 1, int _m = 1, int _y = 2000) : d(_d), m(_m), y(_y) {}
    Ngay(const Ngay& o) : d(o.d), m(o.m), y(o.y) {}
    ~Ngay() {}

    Ngay homNay(){
        time_t t = time(nullptr);
        tm* lt = localtime(&t);
        return Ngay(lt->tm_mday, lt->tm_mon + 1, lt->tm_year + 1900);
    }

    int soSanh(int d1, int m1, int y1, int d2, int m2, int y2){
        if (y1 != y2) return y1 < y2 ? -1 : 1;
        if (m1 != m2) return m1 < m2 ? -1 : 1;
        if (d1 != d2) return d1 < d2 ? -1 : 1;
        return 0;
    }

    const int TUOI_TOI_THIEU = 18;

    Ngay ngaySinhMuonNhat(){
        Ngay h = homNay();
        return Ngay(h.d, h.m, h.y - TUOI_TOI_THIEU);
    }

    bool hopLe(int d, int m, int y){
        if (y < 1900 || m < 1 || m > 12 || d < 1) return false;
        int ngay[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0) ngay[1] = 29;
        if (d > ngay[m - 1]) return false;
        Ngay h = ngaySinhMuonNhat();
        return soSanh(d, m, y, h.d, h.m, h.y) <= 0;
    }
    string toString() const {return to_string(d) + "/" + to_string(m) + "/" + to_string(y);}
    friend ostream& operator<<(ostream& os, const Ngay& n) {return os << n.toString(); }

    Ngay doc(const string& nhan){
        while (true){
            cout << UI::BE << nhan << UI::RESET;
            string s;
            getline(cin, s);
            for (char& c : s) if (c == '/' || c == '-') c = ' ';
            istringstream ss(s);
            int d, m, y;
            if (ss >> d >> m >> y && hopLe(d, m, y)) return Ngay(d, m, y);
            cout << UI::DO << "  ! Ngày sinh không hợp lệ (ví dụ: 7/4/1997). Nhân viên phải đủ " << TUOI_TOI_THIEU << " tuổi, tức sinh từ 1/1/1900 đến " << ngaySinhMuonNhat() << ".\n" << UI::RESET;
        }
    }
};
