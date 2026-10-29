// Tiện ích console: màu sắc, căn lề, nhập liệu, menu phím mũi tên
#pragma once
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <conio.h>
#endif
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <cctype>
using namespace std;

namespace UI 
{
    // Mã màu ANSI (Windows 10+ hỗ trợ sau khi bật Virtual Terminal)
    const string RESET = "\033[0m", DO = "\033[91m", XANH = "\033[38;2;208;0;118m", VANG = "\033[93m",
                 XANH_DUONG = "\033[94m", TIM = "\033[38;2;92;67;137m", CYAN = "\033[38;2;254;235;211m",
                 TRANG = "\033[97m", XAM = "\033[90m",
                 CHON = "\033[97;48;2;229;6;107m"; // chữ trắng, nền hồng = mục đang chọn

    static bool dangNhapNhanVien = false;
    static bool huyNhapNhanVien = false;

    inline void khoiTao() 
    {
#ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD mode = 0;
        if (GetConsoleMode(h, &mode)) SetConsoleMode(h, mode | 0x0004); // ENABLE_VIRTUAL_TERMINAL_PROCESSING
#endif
    }

    inline void xoaManHinh() { cout << "\033[2J\033[H"; }

    // Độ rộng hiển thị = số ký tự UTF-8 (không đếm byte tiếp theo của ký tự)
    inline int doRong(const string& s) 
    {
        int n = 0;
        for (unsigned char c : s) if ((c & 0xC0) != 0x80) n++;
        return n;
    }

    // Căn lề TRÁI trong ô rộng w (thiếu thì thêm khoảng trắng, dài quá thì cắt)
    inline string can(const string& s, int w) 
    {
        string kq;
        int n = 0;
        for (size_t i = 0; i < s.size() && n < w;) 
        {
            unsigned char c = s[i];
            int len = c < 0x80 ? 1 : (c >> 5) == 6 ? 2 : (c >> 4) == 14 ? 3 : 4;
            kq.append(s, i, len);
            i += len;
            n++;
        }
        return kq + string(w - n, ' ');
    }

    inline string lap(const string& s, int n) 
    {
        string kq;
        for (int i = 0; i < n; i++) kq += s;
        return kq;
    }

    inline string tien(double v) 
    { // 5200000 -> "5,200,000"
        long long x = llround(v);
        bool am = x < 0;
        if (am) x = -x;
        string s = to_string(x), kq;
        int dem = 0;
        for (int i = (int)s.size() - 1; i >= 0; i--) 
        {
            kq += s[i];
            if (++dem % 3 == 0 && i > 0) kq += ',';
        }
        reverse(kq.begin(), kq.end());
        return (am ? "-" : "") + kq;
    }

    inline string thuong(string s) 
    {
        for (auto& c : s) if ((unsigned char)c < 128) c = tolower(c);
        return s;
    }

    inline string cat(const string& s) 
    { // bỏ khoảng trắng đầu/cuối
        size_t a = s.find_first_not_of(" \t\r\n");
        if (a == string::npos) return "";
        size_t b = s.find_last_not_of(" \t\r\n");
        return s.substr(a, b - a + 1);
    }

    inline bool docDongCoEsc(const string& nhan, string& ketQua)
    {
        cout << CYAN << nhan << RESET << flush;
#ifdef _WIN32
        wstring s;
        while (true)
        {   
            int c = _getch();
            // Nhấn ESC
            if (c == 27)
            {
                cout << "\n";
                if (dangNhapNhanVien)
                {
                    huyNhapNhanVien = true;
                    return false;
                }
                // Ngoài thao tác thêm nhân viên: nhập lại
                return false;
            }
            // Nhấn Enter
            if (c == 13)
            {
                cout << "\n";
                int n = WideCharToMultiByte(
                    CP_UTF8, 0, s.data(), (int)s.size(),
                    nullptr, 0, nullptr, nullptr
                );
                ketQua.resize(n);
                if (n > 0)
                {
                    WideCharToMultiByte(
                        CP_UTF8, 0, s.data(), (int)s.size(),
                        &ketQua[0], n, nullptr, nullptr
                    );
                }
                return true;
            }
            // Nhấn Backspace
            if (c == 8)
            {
                if (!s.empty())
                {
                    s.pop_back();
                    cout << "\b \b" << flush;
                }
                continue;
            }
            // Bỏ qua các phím điều hướng
            if (c == 0 || c == 224)
            {
                _getch();
                continue;
            }
            // Ký tự thông thường
            if (c >= 32)
            {
                s.push_back((wchar_t)c);
                wstring kyTu(1, (wchar_t)c);
                char buf[5] = {};
                int len = WideCharToMultiByte(
                    CP_UTF8, 0, kyTu.data(), 1,
                    buf, sizeof(buf), nullptr, nullptr
                );
                if (len > 0) cout.write(buf, len);
                cout.flush();
            }
        }
#else
        if (!getline(cin, ketQua))
            return false;

        return true;
#endif
    }

    // ---- Nhập liệu (text input) ----
    inline string docChuoi(const string& nhan)
    {
        while (true)
        {
            string s;
            if (!docDongCoEsc(nhan, s))
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
            if (!docDongCoEsc(nhan, s))
            {
                if (huyNhapNhanVien) return lo;
                continue;
            }
            istringstream ss(cat(s));
            double v;
            if (ss >> v)
            {
                ss >> ws;
                if (ss.eof() && v >= lo && v <= hi) return v;
            }
            cout << DO << "  ! Giá trị không hợp lệ (cần từ " << lo << (hi < 1e17? " đến " + to_string((long long)hi): " trở lên") << ").\n" << RESET;
        }
    }

    inline void tamDung() 
    {
        cout << XAM << "\nNhấn Enter để quay lại menu..." << RESET;
        string t;
        getline(cin, t);
    }

    // In 1 dòng "nhãn : giá trị", nhãn căn trái
    inline void dong(const string& nhan, const string& giaTri, const string& mau = TRANG) 
    {
        cout << VANG << can(nhan, 22) << RESET << ": " << mau << giaTri << RESET << "\n";
    }

    // ---- Phím mũi tên ----
    const int LEN = 1072, XUONG = 1080, ENTER = 13;
    inline int layPhim() 
    {
#ifdef _WIN32
        int c = _getch();
        if (c == 0 || c == 224) return 1000 + _getch();
        return c;
#else // chỉ để test trên Linux: w = lên, s = xuống, e = Enter
        char c;
        if (!(cin >> c)) return ENTER;
        cin.ignore(100000, '\n');
        return c == 'w' ? LEN : c == 's' ? XUONG : ENTER;
#endif
    }

    // Menu chọn bằng ↑ ↓ + Enter, trả về chỉ số mục được chọn
    inline int chonMenu(const string& tieuDe, const string& phu, const vector<string>& muc) 
    {
        int cur = 0, n = muc.size();
        while (true) {
            xoaManHinh();
            cout << "\n  " << VANG << "** " << tieuDe << " **" << RESET << "\n";
            cout << "  " << XAM << phu << RESET << "\n\n";
            for (int i = 0; i < n; i++) 
            {
                if (i == cur) cout << "  " << CHON << " ► " << can(muc[i], 36) << " " << RESET << "\n";
                else cout << "    " << TRANG << "   " << can(muc[i], 36) << RESET << "\n";
            }
            cout << "\n  " << XAM << "↑ ↓ để di chuyển, Enter để chọn" << RESET << "\n";
            int k = layPhim();
            if (k == LEN) cur = (cur + n - 1) % n;
            else if (k == XUONG) cur = (cur + 1) % n;
            else if (k == ENTER) return cur;
        }
    }
}
