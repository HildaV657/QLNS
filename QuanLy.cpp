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
    double getLuongCoBan() const {return luongCoBan;}
    double getLuongTrachNhiem() const {return luongTrachNhiem;}
    int getSoNgayNghi() const {return soNgayNghi;}
    void setLuongCoBan(double x) {luongCoBan = x;}
    void setLuongTrachNhiem(double x) {luongTrachNhiem = x;}
    void setSoNgayNghi(int x) {soNgayNghi = x;}
    QuanLy(const string& ma = "", const string& ten = "", const Ngay& ns = Ngay(), const string& dc = "", double lcb = 0, double ltn = 0, int soNgayNghi = 0)
        : NhanVien(ma, ten, ns, dc), luongCoBan(lcb), luongTrachNhiem(ltn), soNgayNghi(soNgayNghi) {}
    QuanLy(const QuanLy& o) : NhanVien(o), luongCoBan(o.luongCoBan), luongTrachNhiem(o.luongTrachNhiem), soNgayNghi(o.soNgayNghi) {}
    ~QuanLy() {}

    double tinhLuong() const
    {
        if (soNgayNghi > 1) {return luongCoBan + luongTrachNhiem * 0.5;}
        return luongCoBan + luongTrachNhiem;
    }
    string loaiNV() const {return "Quản lý";}
    NhanVien* clone() const {return new QuanLy(*this);}
    string ghiFile() const
    {
        return ghiChung("QL") + "\t" + so(luongCoBan) + "\t" + so(luongTrachNhiem) + "\t" + to_string(soNgayNghi);
    }

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

    void hienThiThongTin(int thang) const override
    {
        NhanVien::hienThiThongTin(thang);
        UI::dong("Lương cơ bản", UI::tien(luongCoBan) + " VNĐ");
        UI::dong("Lương trách nhiệm", UI::tien(luongTrachNhiem) + " VNĐ");
        UI::dong("Số ngày nghỉ", to_string(soNgayNghi) + " ngày");

        if (soNgayNghi > 1)
        {
            if (soNgayNghi == 2){
                UI::dong("Tiền phạt",
                UI::tien(luongTrachNhiem * 0.25) + " VNĐ");
            }
            if (soNgayNghi == 3){
                UI::dong("Tiền phạt",
                UI::tien(luongTrachNhiem * 0.5) + " VNĐ");
            }
            if (soNgayNghi == 4)
                UI::dong("Tiền phạt", "0 VNĐ");
            if (soNgayNghi > 4){
                UI::dong("Tiền phạt",
                UI::tien(luongCoBan - (soNgayNghi * luongTrachNhiem)) + " VNĐ");
            }
        }
        else
        {
            UI::dong("Tiền phạt", "0 VNĐ");
        }

        inLuong(thang);
    }
};
