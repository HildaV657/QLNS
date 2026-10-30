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

    static string sach(string s) // bỏ TAB / xuống dòng để không phá định dạng file
    {
        for (auto& c : s) if (c == '\t' || c == '\n' || c == '\r') c = ' ';
        return s;
    }
    static string so(double v)
    {
        ostringstream os;
        os << setprecision(15) << v;
        return os.str();
    }
    string ghiChung(const string& ma) const
    {
        return ma + "\t" + sach(maNV) + "\t" + sach(hoTen) + "\t" + ngaySinh.toString() + "\t" + sach(diaChi);
    }

public:
    NhanVien(const string& _ma = "", const string& _ten = "", const Ngay& _ns = Ngay(), const string& _dc = "")
        : maNV(_ma), hoTen(_ten), diaChi(_dc), ngaySinh(_ns) {}
    NhanVien(const NhanVien& o) : maNV(o.maNV), hoTen(o.hoTen), diaChi(o.diaChi), ngaySinh(o.ngaySinh) {}
    virtual ~NhanVien() {}

    // get - set
    string getMaNV() const { return maNV; }
    string getHoTen() const { return hoTen; }
    Ngay getNgaySinh() const { return ngaySinh; }
    string getDiaChi() const { return diaChi; }
    void setMaNV(const string& x) { maNV = x; }
    void setHoTen(const string& x) { hoTen = x; }
    void setNgaySinh(const Ngay& x) { ngaySinh = x; }
    void setDiaChi(const string& x) { diaChi = x; }

    // ĐA HÌNH: mỗi loại nhân viên tính lương khác nhau
    virtual double tinhLuong() const = 0;
    virtual string loaiNV() const = 0;
    virtual NhanVien* clone() const = 0;
    // Ghi 1 nhân viên thành 1 dòng văn bản (các trường cách nhau bằng TAB)
    virtual string ghiFile() const = 0;

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
