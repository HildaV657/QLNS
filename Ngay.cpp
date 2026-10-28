#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include "ConsoleUI.cpp"
using namespace std;

class Ngay {
    int d, m, y;
public:
    Ngay(int _d = 1, int _m = 1, int _y = 2000) : d(_d), m(_m), y(_y) {}
    Ngay(const Ngay& o) : d(o.d), m(o.m), y(o.y) {}
    ~Ngay() {}
    int getD() const { return d; }
    int getM() const { return m; }
    int getY() const { return y; }
    void setD(int x) { d = x; }
    void setM(int x) { m = x; }
    void setY(int x) { y = x; }
    Ngay& operator=(const Ngay& o) { d = o.d; m = o.m; y = o.y; return *this; }

    static bool hopLe(int d, int m, int y) {
        if (y < 1900 || y > 2100 || m < 1 || m > 12 || d < 1) return false;
        int ngay[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0) ngay[1] = 29;
        return d <= ngay[m - 1];
    }
    string toString() const { return to_string(d) + "/" + to_string(m) + "/" + to_string(y); }
    friend ostream& operator<<(ostream& os, const Ngay& n) { return os << n.toString(); }

    static Ngay doc(const string& nhan) { // nhập dạng d/m/yyyy
        while (true) {
            cout << UI::CYAN << nhan << UI::RESET;
            string s;
            getline(cin, s);
            for (auto& c : s) if (c == '/' || c == '-') c = ' ';
            istringstream ss(s);
            int d, m, y;
            if (ss >> d >> m >> y && hopLe(d, m, y)) return Ngay(d, m, y);
            cout << UI::DO << "  ! Ngày không hợp lệ (ví dụ: 7/4/1997).\n" << UI::RESET;
        }
    }
};
