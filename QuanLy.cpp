#pragma once
#include "NhanVien.cpp"
using namespace std;

class QuanLy : public NhanVien
{
    double luongCoBan;
    double luongTrachNhiem;
    int soNgayNghi;
public:
    double getLuongCoBan() const {return luongCoBan;}
    double getLuongTrachNhiem() const {return luongTrachNhiem;}
    int getSoNgayNghi() const {return soNgayNghi;}
    void setLuongCoBan(double x) {luongCoBan = x;}
    void setLuongTrachNhiem(double x) {luongTrachNhiem = x;}
    void setSoNgayNghi(int x) {soNgayNghi = x;}
    QuanLy(const string& ma = "ma nhan vien", const string& ten = "ho ten", const Ngay& ns = Ngay(), const string& dc = "dia chi", double lcb = 0, double ltn = 0, int soNgayNghi = 0)
        : NhanVien(ma, ten, ns, dc), luongCoBan(lcb), luongTrachNhiem(ltn), soNgayNghi(soNgayNghi) {}
    QuanLy(const QuanLy& o) : NhanVien(o), luongCoBan(o.luongCoBan), luongTrachNhiem(o.luongTrachNhiem), soNgayNghi(o.soNgayNghi) {}
    ~QuanLy() {}

    double tienPhat() const
    {
        if (soNgayNghi <= 1) return 0;
        if (soNgayNghi == 2) return luongTrachNhiem * 0.25;
        if (soNgayNghi == 3) return luongTrachNhiem * 0.5;
        return luongTrachNhiem;
    }

    double tinhLuong() const
    {
        return luongCoBan + luongTrachNhiem - tienPhat();
    }

    string loaiNV() const {return "Quản lý";}

    void nhap()
    {
        NhanVien::nhap();
        if (UI::huyNhapNhanVien) return;
        luongCoBan = UI::docSo("Lương cơ bản (VNĐ): ", 0);
        if (UI::huyNhapNhanVien) return;
        luongTrachNhiem = UI::docSo("Lương trách nhiệm (VNĐ): ", 0);
        if (UI::huyNhapNhanVien) return;
        soNgayNghi = (int)UI::docSo("Số ngày nghỉ trong tháng: ", 0, 31);
        if (UI::huyNhapNhanVien) return;
    }

    void hienThiThongTin(int thang) const
    {
        NhanVien::hienThiThongTin(thang);
        UI::dong("Lương cơ bản", UI::tien(luongCoBan) + " VNĐ");
        UI::dong("Lương trách nhiệm", UI::tien(luongTrachNhiem) + " VNĐ");
        UI::dong("Số ngày nghỉ", to_string(soNgayNghi) + " ngày");
        UI::dong("Tiền phạt", UI::tien(tienPhat()) + " VNĐ");
        inLuong(thang);
    }
};
