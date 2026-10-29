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
    Ngay(int _d = 1, int _m = 1, int _y = 2000) : d(_d), m(_m), y(_y) {}
    Ngay(const Ngay& o) : d(o.d), m(o.m), y(o.y) {}
    ~Ngay() {}
    int getD() const {return d;}
    int getM() const {return m;}
    int getY() const {return y;}
    void setD(int x) {d = x;}
    void setM(int x) {m = x;}
    void setY(int x) {y = x;}
    Ngay& operator = (const Ngay& o){
        d = o.d; 
        m = o.m; 
        y = o.y; 
        return *this;
    }

    static Ngay homNay(){
        time_t t = time(nullptr);
        tm* lt = localtime(&t);
        return Ngay(lt->tm_mday, lt->tm_mon + 1, lt->tm_year + 1900);
    }
    
    static int soSanh(int d1, int m1, int y1, int d2, int m2, int y2){
        if (y1 != y2) return y1 < y2 ? -1 : 1;
        if (m1 != m2) return m1 < m2 ? -1 : 1;
        if (d1 != d2) return d1 < d2 ? -1 : 1;
        return 0;
    }
    
    static bool hopLe(int d, int m, int y){
        if (y < 1900 || m < 1 || m > 12 || d < 1) return false;
        int ngay[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0) ngay[1] = 29;
        if (d > ngay[m - 1]) return false;
        Ngay h = homNay();
        return soSanh(d, m, y, h.d, h.m, h.y) <= 0;
    }
    string toString() const {return to_string(d) + "/" + to_string(m) + "/" + to_string(y);}
    friend ostream& operator<<(ostream& os, const Ngay& n) { return os << n.toString(); }

    static Ngay doc(const string& nhan){
        while (true){
            cout << UI::CYAN << nhan << UI::RESET;
            string s;
            getline(cin, s);
            for (auto& c : s) if (c == '/' || c == '-') c = ' ';
            istringstream ss(s);
            int d, m, y;
            if (ss >> d >> m >> y && hopLe(d, m, y)) return Ngay(d, m, y);
            cout << UI::DO << "  ! Ngày không hợp lệ (ví dụ: 7/4/1997). Chỉ nhận từ 1/1/1900 đến hôm nay (" << homNay() << ").\n" << UI::RESET;
        }
    }
};