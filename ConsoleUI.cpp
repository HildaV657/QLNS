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
    const string RESET = "\033[0m", DO = "\033[91m", HONG = "\033[38;2;208;0;118m", VANG = "\033[93m",
                 BLUE = "\033[94m", TIM = "\033[38;2;92;67;137m", BE = "\033[38;2;254;235;211m",
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

    // Đổi về chữ thường, kể cả chữ có dấu tiếng Việt (Đ -> đ, Ầ -> ầ, Ư -> ư, ...)
    inline string thuong(const string& s)
    {
        string kq;
        for (size_t i = 0; i < s.size();)
        {
            unsigned char c = s[i];
            int len = c < 0x80 ? 1 : (c >> 5) == 6 ? 2 : (c >> 4) == 14 ? 3 : 4;
            if (i + len > s.size()) len = 1; // chuỗi UTF-8 bị cụt: giữ nguyên byte
            unsigned cp = c;
            if (len == 2) cp = ((c & 0x1F) << 6) | (s[i + 1] & 0x3F);
            else if (len == 3) cp = ((c & 0x0F) << 12) | ((s[i + 1] & 0x3F) << 6) | (s[i + 2] & 0x3F);
            else if (len == 4) cp = 0;
            if (len == 1) kq += (char)(c < 0x80 ? tolower(c) : c);
            else if (len == 4) kq.append(s, i, len);
            else
            {
                if (cp >= 0xC0 && cp <= 0xDE && cp != 0xD7) cp += 32;           // À-Þ -> à-þ (Â, Ê, Ô, Á, ...)
                else if (cp >= 0x100 && cp <= 0x137 && cp % 2 == 0) cp += 1;    // Ă, Đ, Ĩ, Ũ, ...
                else if (cp >= 0x14A && cp <= 0x177 && cp % 2 == 0) cp += 1;    // Ũ, ...
                else if (cp == 0x1A0 || cp == 0x1AF) cp += 1;                    // Ơ, Ư
                else if (cp >= 0x1EA0 && cp <= 0x1EF8 && cp % 2 == 0) cp += 1;   // Ạ Ả Ấ Ầ Ẩ ... Ỹ
                if (cp < 0x800) { kq += (char)(0xC0 | (cp >> 6)); kq += (char)(0x80 | (cp & 0x3F)); }
                else { kq += (char)(0xE0 | (cp >> 12)); kq += (char)(0x80 | ((cp >> 6) & 0x3F)); kq += (char)(0x80 | (cp & 0x3F)); }
            }
            i += len;
        }
        return kq;
    }

    // Chữ thường + bỏ dấu tiếng Việt: "Nguyễn Thị Đức" -> "nguyen thi duc" (dùng để tìm kiếm không phân biệt dấu)
    inline string boDau(const string& s)
    {
        static const char* nhom[] = {
            "aàáảãạăằắẳẵặâầấẩẫậ", "eèéẻẽẹêềếểễệ", "iìíỉĩị", "oòóỏõọôồốổỗộơờớởỡợ",
            "uùúủũụưừứửữự", "yỳýỷỹỵ", "dđ"
        };
        string t = thuong(s), kq;
        for (size_t i = 0; i < t.size();)
        {
            unsigned char c = t[i];
            int len = c < 0x80 ? 1 : (c >> 5) == 6 ? 2 : (c >> 4) == 14 ? 3 : 4;
            if (i + len > t.size()) len = 1;
            string ch = t.substr(i, len);
            char goc = 0;
            if (len > 1)
                for (auto g : nhom)
                {
                    string gs = g; // ký tự đầu là chữ gốc, các ký tự sau là biến thể có dấu
                    if (gs.find(ch, 1) != string::npos) { goc = gs[0]; break; }
                }
            if (goc) kq += goc; else kq += ch;
            i += len;
        }
        return kq;
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
        cout << BE << nhan << RESET << flush;
#ifdef _WIN32
        HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
        DWORD cheDo = 0;
        if (!GetConsoleMode(hIn, &cheDo)) // không phải console (vd: chuyển hướng file) -> đọc cả dòng
        {
            if (!getline(cin, ketQua)) return false;
            return true;
        }
        wstring s;
        while (true)
        {
            // Đọc sự kiện bàn phím thô. KHÔNG dùng _getch/_getwch vì chữ "à" (U+00E0) trùng mã 224 của phím mũi tên,
            // làm "hoàn thành" bị nuốt mất "àn". Phím điều hướng/Shift/Ctrl có UnicodeChar = 0 nên bỏ qua dễ dàng.
            INPUT_RECORD ir;
            DWORD soSuKien = 0;
            if (!ReadConsoleInputW(hIn, &ir, 1, &soSuKien) || soSuKien == 0) return false;
            if (ir.EventType != KEY_EVENT || !ir.Event.KeyEvent.bKeyDown) continue;
            wchar_t c = ir.Event.KeyEvent.uChar.UnicodeChar;
            if (c == 0) continue;
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
                    // ký tự ngoài BMP (emoji...) gồm 2 đơn vị UTF-16 nhưng chỉ chiếm 1 ô trên màn hình
                    if (s.size() >= 2 && s.back() >= 0xDC00 && s.back() <= 0xDFFF) s.pop_back();
                    s.pop_back();
                    cout << "\b \b" << flush;
                }
                continue;
            }
            // Ký tự thông thường
            if (c >= 32)
            {
                s.push_back(c);
                if (c >= 0xD800 && c <= 0xDBFF) continue; // nửa đầu cặp surrogate: đợi nửa sau rồi mới in
                wstring kyTu = (c >= 0xDC00 && c <= 0xDFFF && s.size() >= 2) ? s.substr(s.size() - 2) : wstring(1, c);
                char buf[8] = {};
                int len = WideCharToMultiByte(
                    CP_UTF8, 0, kyTu.data(), (int)kyTu.size(),
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
        cout << VANG << can(nhan, 25) << RESET << ": " << mau << giaTri << RESET << "\n";
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
