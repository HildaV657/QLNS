// NhanVienSanXuat: lương theo sản phẩm, vượt 50 thì giá khác
#pragma once
#include "NhanVien.cpp"
using namespace std;

class NhanVienSanXuat : public NhanVien 
{
    int soSanPham;
    double donGiaSP;     // đơn giá cho sản phẩm trong định mức
    double donGiaVuot;   // đơn giá (thưởng) cho sản phẩm vượt định mức
    static const int DINH_MUC = 50;
public:
    NhanVienSanXuat(const string& ma = "", const string& ten = "", const Ngay& ns = Ngay(), const string& dc = "",
                    int sp = 0, double gia = 0, double giaVuot = 0)
        : NhanVien(ma, ten, ns, dc), soSanPham(sp), donGiaSP(gia), donGiaVuot(giaVuot) {}
    NhanVienSanXuat(const NhanVienSanXuat& o)
        : NhanVien(o), soSanPham(o.soSanPham), donGiaSP(o.donGiaSP), donGiaVuot(o.donGiaVuot) {}
    ~NhanVienSanXuat() {}

    int getSoSanPham() const { return soSanPham; }
    double getDonGiaSP() const { return donGiaSP; }
    double getDonGiaVuot() const { return donGiaVuot; }
    void setSoSanPham(int x) { soSanPham = x; }
    void setDonGiaSP(double x) { donGiaSP = x; }
    void setDonGiaVuot(double x) { donGiaVuot = x; }

    int spTrongDinhMuc() const { return soSanPham < DINH_MUC ? soSanPham : DINH_MUC; }
    int spVuotDinhMuc() const { return soSanPham > DINH_MUC ? soSanPham - DINH_MUC : 0; }

    double tinhLuong() const override { return spTrongDinhMuc() * donGiaSP + spVuotDinhMuc() * donGiaVuot; }
    string loaiNV() const override { return "Sản xuất"; }
    NhanVien* clone() const override { return new NhanVienSanXuat(*this); }

    void nhap() override
    {
        NhanVien::nhap();
        if (UI::huyNhapNhanVien) return;
        soSanPham = (int)UI::docSo("Số sản phẩm làm được: ", 0);
        if (UI::huyNhapNhanVien) return;
        donGiaSP = UI::docSo("Đơn giá sản phẩm (50 đầu tiên): ", 0);
        if (UI::huyNhapNhanVien) return;
        donGiaVuot = UI::docSo("Đơn giá sản phẩm từ 51 trở đi: ", 0);
        if (UI::huyNhapNhanVien) return;
    }
    void hienThiThongTin(int thang) const override 
    {
        NhanVien::hienThiThongTin(thang);
        UI::dong("Tổng số sản phẩm", to_string(soSanPham));
        UI::dong("SP trong định mức", to_string(spTrongDinhMuc()) + " x " + UI::tien(donGiaSP) + " VNĐ");
        UI::dong("SP vượt định mức", to_string(spVuotDinhMuc()) + " x " + UI::tien(donGiaVuot) + " VNĐ");
        inLuong(thang);
    }
};
