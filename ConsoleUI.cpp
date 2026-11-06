#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <conio.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cmath>
using namespace std;

namespace UI
{
    const string RESET = "\033[0m", DO = "\033[91m", VANG = "\033[93m", TRANG = "\033[97m", XAM = "\033[90m";
    const string HONG = "\033[38;2;208;0;118m", TIM = "\033[38;2;92;67;137m", BE = "\033[38;2;254;235;211m";
    const string CHON = "\033[97;48;2;229;6;107m";

    inline bool dangNhapNhanVien = false;
    inline bool huyNhapNhanVien = false;

    inline void khoiTao()
    {
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD mode = 0;
        GetConsoleMode(h, &mode);
        SetConsoleMode(h, mode | 0x0004);
    }

    inline void xoaManHinh() { cout << "\033[2J\033[H"; }

    inline int doDaiKyTu(unsigned char c)
    {
        return c < 0x80 ? 1 : (c >> 5) == 6 ? 2 : (c >> 4) == 14 ? 3 : 4;
    }

    inline int doRong(const string& s)
    {
        int n = 0;
        for (unsigned char c : s) if ((c & 0xC0) != 0x80) n++;
        return n;
    }

    inline string can(const string& s, int w)
    {
        string kq;
        int n = 0;
        for (size_t i = 0; i < s.size() && n < w; n++)
        {
            int len = doDaiKyTu(s[i]);
            kq.append(s, i, len);
            i += len;
        }
        return kq + string(w - n, ' ');
    }

    inline string giua(const string& s, int w)
    {
        int n = doRong(s);
        if (n >= w) return can(s, w);
        int trai = (w - n) / 2;
        return string(trai, ' ') + s + string(w - n - trai, ' ');
    }

    inline string phai(const string& s, int w)
    {
        int n = doRong(s);
        if (n >= w) return s;
        return string(w - n, ' ') + s;
    }

    inline string lap(const string& s, int n)
    {
        string kq;
        for (int i = 0; i < n; i++) kq += s;
        return kq;
    }

    inline string cat(const string& s)
    {
        size_t a = s.find_first_not_of(" \t\r\n");
        if (a == string::npos) return "";
        size_t b = s.find_last_not_of(" \t\r\n");
        return s.substr(a, b - a + 1);
    }

    inline string tien(double v)
    {
        long long x = llround(v);
        bool am = x < 0;
        if (am) x = -x;
        string s = to_string(x), kq;
        for (int i = 0; i < (int)s.size(); i++)
        {
            if (i > 0 && (s.size() - i) % 3 == 0) kq += ',';
            kq += s[i];
        }
        return (am ? "-" : "") + kq;
    }

    inline string utf8(const wstring& w)
    {
        if (w.empty()) return "";
        int n = WideCharToMultiByte(CP_UTF8, 0, w.data(), (int)w.size(), nullptr, 0, nullptr, nullptr);
        string s(n, 0);
        WideCharToMultiByte(CP_UTF8, 0, w.data(), (int)w.size(), &s[0], n, nullptr, nullptr);
        return s;
    }

    inline bool docDong(const string& nhan, string& ketQua)
    {
        cout << BE << nhan << RESET << flush;
        HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        wstring s;
        while (true)
        {
            INPUT_RECORD ir;
            DWORD n;
            if (!ReadConsoleInputW(hIn, &ir, 1, &n) || n == 0) return false;
            if (ir.EventType != KEY_EVENT || !ir.Event.KeyEvent.bKeyDown) continue;
            wchar_t c = ir.Event.KeyEvent.uChar.UnicodeChar;
            if (c == 0) continue;

            if (c == 27)
            {
                cout << "\n";
                if (dangNhapNhanVien) huyNhapNhanVien = true;
                return false;
            }
            if (c == 13) 
            {
                cout << "\n";
                ketQua = utf8(s);
                return true;
            }
            if (c == 8) 
            {
                if (!s.empty())
                {
                    s.pop_back();
                    cout << "\b \b" << flush;
                }
            }
            else if (c >= 32)
            {
                s.push_back(c);
                WriteConsoleW(hOut, &c, 1, &n, nullptr);
            }
        }
    }

    inline string docChuoi(const string& nhan)
    {
        while (true)
        {
            string s;
            if (!docDong(nhan, s))
            {
                if (huyNhapNhanVien) return "";
                continue;
            }
            s = cat(s);
            if (!s.empty()) return s;
            cout << DO << "  ! Không được để trống.\n" << RESET;
        }
    }

    inline double docSo(const string& nhan, double lo, double hi = 1e18)
    {
        while (true)
        {
            string s;
            if (!docDong(nhan, s))
            {
                if (huyNhapNhanVien) return lo;
                continue;
            }
            istringstream ss(cat(s));
            double v;
            char thua;
            if ((ss >> v) && !(ss >> thua) && v >= lo && v <= hi) return v;

            cout << DO << "  ! Giá trị không hợp lệ (cần từ " << lo;
            if (hi < 1e17) cout << " đến " << hi;
            else cout << " trở lên";
            cout << ").\n" << RESET;
        }
    }

    inline void tamDung()
    {
        cout << XAM << "\nNhấn Enter để quay lại menu..." << RESET;
        string t;
        getline(cin, t);
    }

    inline void dong(const string& nhan, const string& giaTri, const string& mau = TRANG)
    {
        cout << VANG << can(nhan, 25) << RESET << ": " << mau << giaTri << RESET << "\n";
    }

    inline int chonMenu(const string& tieuDe, const string& phu, const vector<string>& muc)
    {
        int cur = 0, n = muc.size();
        while (true)
        {
            xoaManHinh();
            cout << "\n  " << VANG << "** " << tieuDe << " **" << RESET << "\n";
            cout << "  " << XAM << phu << RESET << "\n\n";
            for (int i = 0; i < n; i++)
            {
                if (i == cur) cout << "  " << CHON << " ► " << can(muc[i], 36) << " " << RESET << "\n";
                else cout << "    " << TRANG << "   " << can(muc[i], 36) << RESET << "\n";
            }
            cout << "\n  " << XAM << "↑ ↓ để di chuyển, Enter để chọn" << RESET << "\n";

            int k = _getch();
            if (k == 0 || k == 224)
            {
                k = _getch();
                if (k == 72) cur = (cur + n - 1) % n;      
                else if (k == 80) cur = (cur + 1) % n;     
            }
            else if (k == 13) return cur;                  
        }
    }
}
