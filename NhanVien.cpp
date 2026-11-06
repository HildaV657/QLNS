#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include "ConsoleUI.cpp"
#include "Ngay.cpp"
using namespace std;

class NhanVien
{
protected:
    string maNV, hoTen, diaChi;
    Ngay ngaySinh;

    void inLuong(int thang) const
    {
        UI::dong("Lương tháng " + to_string(thang), UI::tien(tinhLuong()) + " VNĐ", UI::HONG);
    }
public:
    string getMaNV() const {return maNV;}
    string getHoTen() const {return hoTen;}
    Ngay getNgaySinh() const {return ngaySinh;}
    string getDiaChi() const {return diaChi;}
    void setMaNV(const string& x) {maNV = x;}
    void setHoTen(const string& x) {hoTen = x;}
    void setNgaySinh(const Ngay& x) {ngaySinh = x;}
    void setDiaChi(const string& x) {diaChi = x;}

    NhanVien(const string& _ma = "ma nhan vien", const string& _ten = "ho ten", const Ngay& _ns = Ngay(), const string& _dc = "dia chi")
        : maNV(_ma), hoTen(_ten), diaChi(_dc), ngaySinh(_ns) {}
    NhanVien(const NhanVien& o) : maNV(o.maNV), hoTen(o.hoTen), diaChi(o.diaChi), ngaySinh(o.ngaySinh) {}
    virtual ~NhanVien() {}

    virtual double tinhLuong() const = 0;
    virtual string loaiNV() const = 0;

    virtual void nhap()
    {
        hoTen = UI::docChuoi("Họ và tên: ");
        if (UI::huyNhapNhanVien) return;
        ngaySinh = Ngay::doc("Ngày sinh (d/m/yyyy): ");
        if (UI::huyNhapNhanVien) return;
        diaChi = UI::docChuoi("Địa chỉ: ");
        if (UI::huyNhapNhanVien) return;
    }

    virtual void hienThiThongTin(int thang) const
    {
        UI::dong("Mã nhân viên", maNV);
        UI::dong("Họ và tên", hoTen);
        UI::dong("Ngày sinh", ngaySinh.toString());
        UI::dong("Địa chỉ", diaChi);
        UI::dong("Loại nhân viên", loaiNV(), UI::BE);
    }

    friend istream& operator>>(istream& is, NhanVien& nv) { nv.nhap(); return is; }
    friend ostream& operator<<(ostream& os, const NhanVien& nv)
    {
        return os << nv.maNV << " - " << nv.hoTen << " (" << nv.loaiNV() << ")";
    }
};
