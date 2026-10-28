#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include "NhanVien.cpp"
#include "NhanVienVanPhong.cpp"
#include "NhanVienSanXuat.cpp"
#include "QuanLy.cpp"
using namespace std;

class DanhSachNhanVien {
    vector<NhanVien*> ds;
    int thang;

    // độ rộng các cột của bảng
    const vector<int> W = {5, 8, 24, 12, 12, 16};

    void vienNgang(const string& l, const string& g, const string& r) const {
        cout << UI::TRANG << l;
        for (size_t i = 0; i < W.size(); i++) cout << UI::lap("─", W[i] + 2) << (i + 1 < W.size() ? g : r);
        cout << UI::RESET << "\n";
    }

    // Mỗi ô căn lề trái, nằm gọn trong khung
    void dongBang(const vector<string>& o, const string& mau) const {
        for (size_t i = 0; i < W.size(); i++)
            cout << UI::TRANG << "│ " << mau << UI::can(o[i], W[i]) << UI::TRANG << " ";
        cout << "│" << UI::RESET << "\n";
    }

public:
    DanhSachNhanVien(int t = 1) : thang(t) {}
    DanhSachNhanVien(const DanhSachNhanVien& o) : thang(o.thang) {
        for (auto p : o.ds) ds.push_back(p->clone()); // sao chép sâu
    }
    DanhSachNhanVien& operator=(const DanhSachNhanVien&) = delete;
    ~DanhSachNhanVien() {
        for (auto p : ds) delete p;
    }

    int getThang() const { return thang; }
    void setThang(int t) { thang = t; }
    int soLuong() const { return ds.size(); }

    NhanVien* timTheoMa(const string& ma) const {
        for (auto p : ds) if (UI::thuong(p->getMaNV()) == UI::thuong(ma)) return p;
        return nullptr;
    }

    void them(NhanVien* p) { ds.push_back(p); }

    bool xoa(const string& ma) {
        for (auto it = ds.begin(); it != ds.end(); ++it)
            if (UI::thuong((*it)->getMaNV()) == UI::thuong(ma)) {
                delete *it;
                ds.erase(it);
                return true;
            }
        return false;
    }

    vector<NhanVien*> timTheoTuKhoa(const string& tk) const {
        vector<NhanVien*> kq;
        string k = UI::thuong(tk);
        for (auto p : ds) {
            string gop = UI::thuong(p->getMaNV() + " " + p->getHoTen() + " " + p->loaiNV() + " " + p->getDiaChi());
            if (gop.find(k) != string::npos) kq.push_back(p);
        }
        return kq;
    }

    void sapXepLuongGiam() {
        stable_sort(ds.begin(), ds.end(), [](NhanVien* a, NhanVien* b) { return a->tinhLuong() > b->tinhLuong(); });
    }

    double tongLuong() const {
        double s = 0;
        for (auto p : ds) s += p->tinhLuong();
        return s;
    }

    void inBang(const vector<NhanVien*>& v, const string& tieuDe) const {
        cout << "\n  " << UI::VANG << "** " << tieuDe << " - THÁNG " << thang << " **" << UI::RESET << "\n\n";
        vienNgang("┌", "┬", "┐");
        dongBang({"STT", "Mã NV", "Họ và tên", "Loại", "Ngày sinh", "Lương (VNĐ)"}, UI::VANG);
        vienNgang("├", "┼", "┤");
        int stt = 1;
        for (auto p : v) {
            string mau = p->loaiNV() == "Quản lý" ? UI::TIM : p->loaiNV() == "Sản xuất" ? UI::XANH : UI::CYAN;
            dongBang({to_string(stt++), p->getMaNV(), p->getHoTen(), p->loaiNV(), p->getNgaySinh().toString(), UI::tien(p->tinhLuong())}, mau);
        }
        vienNgang("└", "┴", "┘");
        cout << UI::XAM << "  Tổng: " << v.size() << " nhân viên" << UI::RESET << "\n";
    }

    void inTatCa() const{inBang(ds, "DANH SÁCH NHÂN VIÊN");}

    void thongKe() const {
        cout << "\n  " << UI::VANG << "** THỐNG KÊ THÁNG " << thang << " **" << UI::RESET << "\n\n";
        int vp = 0, sx = 0, ql = 0;
        const NhanVien* max = nullptr;
        for (auto p : ds) {
            string l = p->loaiNV();
            if (l == "Văn phòng") vp++; else if (l == "Sản xuất") sx++; else ql++;
            if (!max || p->tinhLuong() > max->tinhLuong()) max = p;
        }
        UI::dong("Tổng số nhân viên", to_string(ds.size()));
        UI::dong("Văn phòng", to_string(vp), UI::CYAN);
        UI::dong("Sản xuất", to_string(sx), UI::XANH);
        UI::dong("Quản lý", to_string(ql), UI::TIM);
        UI::dong("Tổng quỹ lương", UI::tien(tongLuong()) + " VNĐ", UI::XANH);
        if (!ds.empty()) {
            UI::dong("Lương trung bình", UI::tien(tongLuong() / ds.size()) + " VNĐ");
            UI::dong("Lương cao nhất", max->getHoTen() + " - " + UI::tien(max->tinhLuong()) + " VNĐ");
        }
    }

    void napMau() {
        them(new NhanVienVanPhong("VP01", "Nguyễn Thành Thông", Ngay(7, 4, 1997), "Quận 5, TP.HCM", 26, 400000, 10));
        them(new NhanVienVanPhong("VP02", "Lê Tuấn Khải", Ngay(9, 3, 1998), "Thủ Đức, TP.HCM", 24, 350000, 5));
        them(new NhanVienSanXuat("SX01", "Đỗ Nguyên Khang", Ngay(11, 3, 1998), "Bình Dương", 45, 50000, 75000));
        them(new NhanVienSanXuat("SX02", "Nguyễn Huỳnh Đức", Ngay(4, 5, 1998), "Quận 12, TP.HCM", 80, 50000, 75000));
        them(new QuanLy("QL01", "Châu Thị Ngọc", Ngay(2, 1, 1997), "Quận 3, TP.HCM", 12000000, 3000000));
    }
};
