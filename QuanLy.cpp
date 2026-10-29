// QuanLy: lương cơ bản + lương trách nhiệm
#pragma once
#include "NhanVien.cpp"
using namespace std;

class QuanLy : public NhanVien 
{
    double luongCoBan;
    double luongTrachNhiem;
public:
    QuanLy(const string& ma = "", const string& ten = "", const Ngay& ns = Ngay(), const string& dc = "",
           double lcb = 0, double ltn = 0)
        : NhanVien(ma, ten, ns, dc), luongCoBan(lcb), luongTrachNhiem(ltn) {}
    QuanLy(const QuanLy& o) : NhanVien(o), luongCoBan(o.luongCoBan), luongTrachNhiem(o.luongTrachNhiem) {}
    ~QuanLy() {}

    double getLuongCoBan() const { return luongCoBan; }
    double getLuongTrachNhiem() const { return luongTrachNhiem; }
    void setLuongCoBan(double x) { luongCoBan = x; }
    void setLuongTrachNhiem(double x) { luongTrachNhiem = x; }

    double tinhLuong() const override { return luongCoBan + luongTrachNhiem; }
    string loaiNV() const override { return "Quản lý"; }
    NhanVien* clone() const override { return new QuanLy(*this); }

    void nhap() override
    {
        NhanVien::nhap();
        if (UI::huyNhapNhanVien) return;
        luongCoBan = UI::docSo("Lương cơ bản (VNĐ): ", 0);
        if (UI::huyNhapNhanVien) return;
        luongTrachNhiem = UI::docSo("Lương trách nhiệm (VNĐ): ", 0);
        if (UI::huyNhapNhanVien) return;
}

    void hienThiThongTin(int thang) const override 
    {
        NhanVien::hienThiThongTin(thang);
        UI::dong("Lương cơ bản", UI::tien(luongCoBan) + " VNĐ");
        UI::dong("Lương trách nhiệm", UI::tien(luongTrachNhiem) + " VNĐ");
        inLuong(thang);
    }
};
