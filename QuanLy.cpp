// QuanLy: lương cơ bản + lương trách nhiệm
#pragma once
#include "NhanVien.cpp"
using namespace std;

class QuanLy : public NhanVien 
{
    double luongCoBan;
    double luongTrachNhiem;
    int soNgayNghi;
public:
    QuanLy(const string& ma = "", const string& ten = "", const Ngay& ns = Ngay(), const string& dc = "",
           double lcb = 0, double ltn = 0, int soNgayNghi = 0)
        : NhanVien(ma, ten, ns, dc), luongCoBan(lcb), luongTrachNhiem(ltn), soNgayNghi(soNgayNghi) {}
    QuanLy(const QuanLy& o) : NhanVien(o), luongCoBan(o.luongCoBan), luongTrachNhiem(o.luongTrachNhiem), soNgayNghi(o.soNgayNghi) {}
    ~QuanLy() {}

    double getLuongCoBan() const { return luongCoBan; }
    double getLuongTrachNhiem() const { return luongTrachNhiem; }
    int getSoNgayNghi() const { return soNgayNghi; } 
    void setLuongCoBan(double x) { luongCoBan = x; }
    void setLuongTrachNhiem(double x) { luongTrachNhiem = x; }
    void setSoNgayNghi (int x) {soNgayNghi=x;}


    double tinhLuong() const override 
    {
        if (soNgayNghi > 1) { return luongCoBan + luongTrachNhiem * 0.5;}
        return luongCoBan + luongTrachNhiem;
    }
    string loaiNV() const override { return "Quản lý"; }
    NhanVien* clone() const override { return new QuanLy(*this); }
    string ghiFile() const override
    {
        return ghiChung("QL") + "\t" + so(luongCoBan) + "\t" + so(luongTrachNhiem);
    }

    void nhap() override
    {
        NhanVien::nhap();
        if (UI::huyNhapNhanVien) return;
        luongCoBan = UI::docSo("Lương cơ bản (VNĐ): ", 0);
        if (UI::huyNhapNhanVien) return;
        luongTrachNhiem = UI::docSo("Lương trách nhiệm (VNĐ): ", 0);
        if (UI::huyNhapNhanVien) return;
        soNgayNghi = (int)UI::docSo("Số Ngày Nghỉ trong tháng: ", 0, 31);
        if (UI::huyNhapNhanVien) return;
    }

    void hienThiThongTin(int thang) const override 
    {
        NhanVien::hienThiThongTin(thang);
        UI::dong("Lương cơ bản", UI::tien(luongCoBan) + " VNĐ");
        UI::dong("Lương trách nhiệm", UI::tien(luongTrachNhiem) + " VNĐ");
        UI::dong("Số ngày nghỉ", to_string(soNgayNghi) + " ngày");
    
        if (soNgayNghi > 1) 
        {
            UI::dong("Tiền phạt",
                     UI::tien(luongTrachNhiem * 0.5) + " VNĐ");
        } 
        else 
        {
            UI::dong("Tiền trách nhiệm bị trừ", "0 VNĐ");
        }
    
        inLuong(thang);
    }
};
