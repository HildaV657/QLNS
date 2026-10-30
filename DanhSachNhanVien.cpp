#pragma once
#include <bits/stdc++.h>
#include "NhanVien.cpp"
#include "NhanVienVanPhong.cpp"
#include "NhanVienSanXuat.cpp"
#include "QuanLy.cpp"
using namespace std;


string layNgayXoa()
{
    time_t bayGio = time(0);
    tm* ngay = localtime(&bayGio);

    stringstream ss;
    ss << setfill('0') << setw(2) << ngay->tm_mday << "/" << setw(2) << ngay->tm_mon + 1 << "/" << ngay->tm_year + 1900;
    return ss.str();
}

class DanhSachNhanVien
{
    vector<NhanVien*> ds;
    int thang;

    // độ rộng các cột của bảng
    const vector<int> W = {5, 8, 24, 12, 12, 16};

    void vienNgang(const string& l, const string& g, const string& r) const
    {
        cout << UI::TRANG << l;
        for (size_t i = 0; i < W.size(); i++) cout << UI::lap("─", W[i] + 2) << (i + 1 < W.size() ? g : r);
        cout << UI::RESET << "\n";
    }

    // Mỗi ô căn lề trái, nằm gọn trong khung
    void dongBang(const vector<string>& o, const string& mau) const
    {
        for (size_t i = 0; i < W.size(); i++)
            cout << UI::TRANG << "│ " << mau << UI::can(o[i], W[i]) << UI::TRANG << " ";
        cout << "│" << UI::RESET << "\n";
    }

public:
    DanhSachNhanVien(int t = 1) : thang(t) {}
    DanhSachNhanVien(const DanhSachNhanVien& o) : thang(o.thang)
    {
        for (auto p : o.ds) ds.push_back(p->clone()); // sao chép sâu
    }
    DanhSachNhanVien& operator=(const DanhSachNhanVien&) = delete;
    ~DanhSachNhanVien()
    {
        for (auto p : ds) delete p;
    }

    int getThang() const { return thang; }
    void setThang(int t) { thang = t; }
    int soLuong() const { return ds.size(); }

    NhanVien* timTheoMa(const string& ma) const
    {
        for (auto p : ds) if (UI::thuong(p->getMaNV()) == UI::thuong(ma)) return p;
        return nullptr;
    }

    void them(NhanVien* p) { ds.push_back(p); }
    //XOA NHAN VIEN

bool xoa(const string& ma, const string& LyDo)
{
    for (auto it = ds.begin(); it != ds.end(); ++it)
    {
        if (UI::thuong((*it)->getMaNV()) == UI::thuong(ma))
        {
            // Lay thong tin truoc khi xoa
            string maNV = (*it)->getMaNV();
            string hoTen = (*it)->getHoTen();
            string ngayXoa = layNgayXoa();
            // Mo file de ghi lich su
            ofstream file("lichsu_xoa.txt", ios::app);
            if (!file.is_open())
            {
                cout << UI::DO
                     << "\n  Khong the mo file lich su xoa!\n"
                     << UI::RESET;
                return false;
            }
            // Luu 4 thong tin, moi nhan vien mot dong
            file << maNV << '\t'
                 << hoTen << '\t'
                 << LyDo << '\t'
                 << ngayXoa << '\n';

            file.close();
            // Chi xoa khi da ghi lich su thanh cong
            if (!file)
            {
                cout << UI::DO
                     << "\n  Khong the ghi lich su xoa!\n"
                     << UI::RESET;
                return false;
            }
            delete *it;
            ds.erase(it);
            return true;
        }
    }
    return false;
}

void xemLichSuXoa() const
{
    ifstream file("lichsu_xoa.txt");
    if (!file.is_open())
    {
        cout << "\n  Chua co lich su xoa nhan vien.\n";
        return;
    }
    string dong;
    int stt = 0;

    cout << "\n"
         << UI::VANG
         << "** LICH SU XOA NHAN VIEN **\n"
         << UI::RESET;

    while (getline(file, dong))
    {
        stringstream ss(dong);

        string maNV, hoTen, lyDo, ngayXoa;

        getline(ss, maNV, '\t');
        getline(ss, hoTen, '\t');
        getline(ss, lyDo, '\t');
        getline(ss, ngayXoa);

        cout << "\nNhan vien thu " << ++stt << '\n';
        cout << "Ma nhan vien: " << maNV << '\n';
        cout << "Ho ten: " << hoTen << '\n';
        cout << "Ly do xoa: " << lyDo << '\n';
        cout << "Ngay xoa: " << ngayXoa << '\n';
    }

    if (stt == 0)
        cout << "\n  Chua co lich su xoa nhan vien.\n";
    file.close();
}

    vector<NhanVien*> timTheoTuKhoa(const string& tk) const
    {
        vector<NhanVien*> kq;
        string k = UI::thuong(tk);
        for (auto p : ds)
        {
            string gop = UI::thuong(p->getMaNV() + " " + p->getHoTen() + " " + p->loaiNV() + " " + p->getDiaChi());
            if (gop.find(k) != string::npos) kq.push_back(p);
        }
        return kq;
    }

    void sapXepLuongGiam()
    {
        stable_sort(ds.begin(), ds.end(), [](NhanVien* a, NhanVien* b) { return a->tinhLuong() > b->tinhLuong(); });
    }

    double tongLuong() const
    {
        double s = 0;
        for (auto p : ds) s += p->tinhLuong();
        return s;
    }

    void inBang(const vector<NhanVien*>& v, const string& tieuDe) const
    {
        cout << "\n  " << UI::VANG << "** " << tieuDe << " - THÁNG " << thang << " **" << UI::RESET << "\n\n";
        vienNgang("┌", "┬", "┐");
        dongBang({"STT", "Mã NV", "Họ và tên", "Loại", "Ngày sinh", "Lương (VNĐ)"}, UI::VANG);
        vienNgang("├", "┼", "┤");
        int stt = 1;
        for (auto p : v)
        {
            string mau = p->loaiNV() == "Quản lý" ? UI::TIM : p->loaiNV() == "Sản xuất" ? UI::HONG : UI::BE;
            dongBang({to_string(stt++), p->getMaNV(), p->getHoTen(), p->loaiNV(),
                      p->getNgaySinh().toString(), UI::tien(p->tinhLuong())}, mau);
        }
        vienNgang("└", "┴", "┘");
        cout << UI::XAM << "  Tổng: " << v.size() << " nhân viên" << UI::RESET << "\n";
    }

    // Thứ tự hiển thị theo loại: Quản lý -> Văn phòng -> Sản xuất
    static int thuTuLoai(const NhanVien* p)
    {
        string l = p->loaiNV();
        if (l == "Quản lý") return 0;
        if (l == "Văn phòng") return 1;
        return 2;
    }

    // nhomTheoLoai = true: gom nhóm QL -> VP -> SX (trong mỗi nhóm giữ nguyên thứ tự hiện có)
    // nhomTheoLoai = false: in đúng thứ tự đang lưu (dùng khi vừa sắp xếp theo lương)
    void inTatCa(bool nhomTheoLoai = true) const
    {
        vector<NhanVien*> v = ds;
        if (nhomTheoLoai)
            stable_sort(v.begin(), v.end(), [](NhanVien* a, NhanVien* b) { return thuTuLoai(a) < thuTuLoai(b); });
        inBang(v, "DANH SÁCH NHÂN VIÊN");
    }

    void thongKe() const
    {
        cout << "\n  " << UI::VANG << "** THỐNG KÊ THÁNG " << thang << " **" << UI::RESET << "\n\n";
        int vp = 0, sx = 0, ql = 0;
        const NhanVien* max = nullptr;
        for (auto p : ds)
        {
            string l = p->loaiNV();
            if (l == "Văn phòng") vp++; else if (l == "Sản xuất") sx++; else ql++;
            if (!max || p->tinhLuong() > max->tinhLuong()) max = p;
        }
        UI::dong("Tổng số nhân viên", to_string(ds.size()));
        UI::dong("Văn phòng", to_string(vp), UI::BE);
        UI::dong("Sản xuất", to_string(sx), UI::HONG);
        UI::dong("Quản lý", to_string(ql), UI::TIM);
        UI::dong("Tổng quỹ lương", UI::tien(tongLuong()) + " VNĐ", UI::HONG);
        if (!ds.empty())
        {
            UI::dong("Lương trung bình", UI::tien(tongLuong() / ds.size()) + " VNĐ");
            UI::dong("Lương cao nhất", max->getHoTen() + " - " + UI::tien(max->tinhLuong()) + " VNĐ");
        }
    }

    // ================= LƯU / ĐỌC FILE TXT =================
    // Mỗi nhân viên 1 dòng, các trường cách nhau bằng TAB:
    //   VP <mã> <tên> <d/m/yyyy> <địa chỉ> <số ngày> <số ngày nghỉ> <đơn giá ngày> <giờ OT> <hệ số OT>
    //   SX <mã> <tên> <d/m/yyyy> <địa chỉ> <số SP> <đơn giá SP> <đơn giá vượt>
    //   QL <mã> <tên> <d/m/yyyy> <địa chỉ> <lương cơ bản> <lương trách nhiệm> <số ngày nghỉ>
    // (file cũ chưa có "số ngày nghỉ" vẫn đọc được, mặc định 0)
    // Dòng trống hoặc bắt đầu bằng '#' sẽ bị bỏ qua.
    bool luuFile(const string& duongDan) const
    {
        ofstream f(duongDan, ios::trunc);
        if (!f.is_open()) return false;
        f << "# QLNS - danh sach nhan vien (cac truong cach nhau bang TAB, luu UTF-8)\n";
        f << "# VP: ma, ten, ngay sinh, dia chi, so ngay, so ngay nghi, don gia ngay, gio OT, he so OT\n";
        f << "# SX: ma, ten, ngay sinh, dia chi, so SP, don gia SP, don gia vuot\n";
        f << "# QL: ma, ten, ngay sinh, dia chi, luong co ban, luong trach nhiem, so ngay nghi\n";
        for (auto p : ds) f << p->ghiFile() << '\n';
        f.flush();
        return (bool)f;
    }

private:
    static vector<string> tach(const string& dong)
    {
        vector<string> v;
        string t;
        stringstream ss(dong);
        while (getline(ss, t, '\t')) v.push_back(t);
        if (!dong.empty() && dong.back() == '\t') v.push_back("");
        return v;
    }
    static bool docSoThuc(const string& s, double& v)
    {
        string c = UI::cat(s);
        if (c.empty()) return false;
        char* e = nullptr;
        v = strtod(c.c_str(), &e);
        return *e == '\0' && isfinite(v) && v >= 0;
    }
    static bool docNgay(const string& s, Ngay& n)
    {
        string t = UI::cat(s);
        for (auto& c : t) if (c == '/' || c == '-') c = ' ';
        istringstream ss(t);
        int d, m, y;
        if (!(ss >> d >> m >> y)) return false;
        string du;
        if (ss >> du) return false;
        if (!Ngay::hopLe(d, m, y)) return false;
        n = Ngay(d, m, y);
        return true;
    }
    // Tạo nhân viên từ 1 dòng; trả nullptr nếu dòng sai định dạng
    static NhanVien* taoTuDong(const vector<string>& f)
    {
        if (f.size() < 7) return nullptr;
        string loai = UI::cat(f[0]), ma = UI::cat(f[1]), ten = UI::cat(f[2]), dc = UI::cat(f[4]);
        if (ma.empty() || ten.empty() || dc.empty()) return nullptr;
        Ngay ns;
        if (!docNgay(f[3], ns)) return nullptr;
        vector<double> x;
        for (size_t i = 5; i < f.size(); i++)
        {
            double v;
            if (!docSoThuc(f[i], v)) return nullptr;
            x.push_back(v);
        }
        if (loai == "VP" && (f.size() == 9 || f.size() == 10))
        {
            bool coNghi = f.size() == 10; // file cũ (9 trường) không có số ngày nghỉ
            double nghi = coNghi ? x[1] : 0;
            size_t k = coNghi ? 2 : 1;    // vị trí của đơn giá ngày
            if (x[0] > 31 || x[0] != floor(x[0]) || nghi > 31 || nghi != floor(nghi) || x[k + 2] < 1) return nullptr;
            return new NhanVienVanPhong(ma, ten, ns, dc, (int)x[0], (int)nghi, x[k], x[k + 1], x[k + 2]);
        }
        if (loai == "SX" && f.size() == 8 && x[0] == floor(x[0]))
            return new NhanVienSanXuat(ma, ten, ns, dc, (int)x[0], x[1], x[2]);
        if (loai == "QL" && (f.size() == 7 || f.size() == 8))
        {
            double nghi = f.size() == 8 ? x[2] : 0;
            if (nghi > 31 || nghi != floor(nghi)) return nullptr;
            return new QuanLy(ma, ten, ns, dc, x[0], x[1], (int)nghi);
        }
        return nullptr;
    }

public:
    // Đọc file, thêm vào danh sách (bỏ qua mã trùng / dòng lỗi).
    // Trả về false nếu không mở được file.
    bool docFile(const string& duongDan, int& themDuoc, int& trung, int& loi)
    {
        themDuoc = trung = loi = 0;
        ifstream f(duongDan);
        if (!f.is_open()) return false;
        string dong;
        bool dau = true;
        while (getline(f, dong))
        {
            if (dau) // bỏ BOM UTF-8 nếu có
            {
                dau = false;
                if (dong.size() >= 3 && (unsigned char)dong[0] == 0xEF && (unsigned char)dong[1] == 0xBB && (unsigned char)dong[2] == 0xBF)
                    dong.erase(0, 3);
            }
            if (!dong.empty() && dong.back() == '\r') dong.pop_back();
            if (UI::cat(dong).empty() || UI::cat(dong)[0] == '#') continue;
            NhanVien* p = taoTuDong(tach(dong));
            if (!p) { loi++; continue; }
            if (timTheoMa(p->getMaNV())) { delete p; trung++; continue; }
            them(p);
            themDuoc++;
        }
        return true;
    }

    void napMau()
    {
        them(new NhanVienVanPhong("VP01", "Nguyễn Thành Thông", Ngay(7, 4, 1997), "Quận 5, TP.HCM", 26, 0, 400000, 10));
        them(new NhanVienVanPhong("VP02", "Lê Tuấn Khải", Ngay(9, 3, 1998), "Thủ Đức, TP.HCM", 24, 0, 350000, 5));
        them(new NhanVienSanXuat("SX01", "Đỗ Nguyên Khang", Ngay(11, 3, 1998), "Bình Dương", 45, 50000, 75000));
        them(new NhanVienSanXuat("SX02", "Nguyễn Huỳnh Đức", Ngay(4, 5, 1998), "Quận 12, TP.HCM", 80, 50000, 75000));
        them(new QuanLy("QL01", "Châu Thị Ngọc", Ngay(2, 1, 1997), "Quận 3, TP.HCM", 12000000, 3000000));
    }
};