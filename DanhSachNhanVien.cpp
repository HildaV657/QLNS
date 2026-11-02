#pragma once
#include <bits/stdc++.h>
#include "NhanVien.cpp"
#include "NhanVienVanPhong.cpp"
#include "NhanVienSanXuat.cpp"
#include "QuanLy.cpp"
using namespace std;

class DanhSachNhanVien
{
    vector<NhanVien*> ds;
    int thang;

    const vector<int> W = {5, 8, 24, 12, 12, 16};

    void vienNgang(const string& l, const string& g, const string& r) const
    {
        cout << UI::TRANG << l;
        for (int i = 0; i < W.size(); i++) cout << UI::lap("─", W[i] + 2) << (i + 1 < W.size() ? g : r);
        cout << UI::RESET << "\n";
    }

    // căn lề trái trong bảng
    void dongBang(const vector<string>& o, const string& mau) const
    {
        for (int i = 0; i < W.size(); i++)
            cout << UI::TRANG << "│ " << mau << UI::can(o[i], W[i]) << UI::TRANG << " ";
        cout << "│" << UI::RESET << "\n";
    }

    // sắp xếp lương giảm dần
    void quickSort(int l, int r)
    {
        int i = l, j = r;
        double x = ds[(l + r) / 2]->tinhLuong();
        while (i <= j)
        {
            while (ds[i]->tinhLuong() > x) i++;
            while (ds[j]->tinhLuong() < x) j--;
            if (i <= j)
            {
                swap(ds[i], ds[j]);
                i++;
                j--;
            }
        }
        if (l < j) quickSort(l, j);
        if (i < r) quickSort(i, r);
    }

public:
    DanhSachNhanVien(int t = 1) : thang(t) {}
    DanhSachNhanVien(const DanhSachNhanVien&) = delete;
    ~DanhSachNhanVien()
    {
        for (int i = 0; i < ds.size(); i++) delete ds[i];
    }

    int getThang() const { return thang; }
    void setThang(int t) { thang = t; }
    int soLuong() const { return ds.size(); }

    void them(NhanVien* p) { ds.push_back(p); }

    NhanVien* timTheoMa(const string& ma) const
    {
        for (int i = 0; i < ds.size(); i++)
            if (ds[i]->getMaNV() == ma) return ds[i];
        return nullptr;
    }

    bool xoa(const string& ma)
    {
        for (int i = 0; i < ds.size(); i++)
        {
            if (ds[i]->getMaNV() == ma)
            {
                delete ds[i];
                ds.erase(ds.begin() + i);
                return true;
            }
        }
        return false;
    }

    void sapXepLuongGiam()
    {
        if (ds.size() > 1) quickSort(0, (int)ds.size() - 1);
    }

    double tongLuong() const
    {
        double s = 0;
        for (int i = 0; i < ds.size(); i++) s += ds[i]->tinhLuong();
        return s;
    }

    void inTatCa() const
    {
        cout << "\n  " << UI::VANG << "** DANH SÁCH NHÂN VIÊN - THÁNG " << thang << " **" << UI::RESET << "\n\n";
        vienNgang("┌", "┬", "┐");
        dongBang({"STT", "Mã NV", "Họ và tên", "Loại", "Ngày sinh", "Lương (VNĐ)"}, UI::VANG);
        vienNgang("├", "┼", "┤");
        for (int i = 0; i < ds.size(); i++)
        {
            NhanVien* p = ds[i];
            string mau = p->loaiNV() == "Quản lý" ? UI::TIM : p->loaiNV() == "Sản xuất" ? UI::HONG : UI::BE;
            dongBang({to_string(i + 1), p->getMaNV(), p->getHoTen(), p->loaiNV(), p->getNgaySinh().toString(), UI::tien(p->tinhLuong())}, mau);
        }
        vienNgang("└", "┴", "┘");
        cout << UI::XAM << "  Tổng: " << ds.size() << " nhân viên" << UI::RESET << "\n";
    }

    void thongKe() const
    {
        cout << "\n  " << UI::VANG << "** THỐNG KÊ THÁNG " << thang << " **" << UI::RESET << "\n\n";
        int vp = 0, sx = 0, ql = 0;
        NhanVien* cao = nullptr; // nhân viên lương cao nhất
        for (int i = 0; i < ds.size(); i++)
        {
            string l = ds[i]->loaiNV();
            if (l == "Văn phòng") vp++;
            else if (l == "Sản xuất") sx++;
            else ql++;
            if (cao == nullptr || ds[i]->tinhLuong() > cao->tinhLuong()) cao = ds[i];
        }
        UI::dong("Tổng số nhân viên", to_string(ds.size()));
        UI::dong("Văn phòng", to_string(vp), UI::BE);
        UI::dong("Sản xuất", to_string(sx), UI::HONG);
        UI::dong("Quản lý", to_string(ql), UI::TIM);
        UI::dong("Tổng quỹ lương", UI::tien(tongLuong()) + " VNĐ", UI::HONG);
        if (cao != nullptr)
        {
            UI::dong("Lương trung bình", UI::tien(tongLuong() / ds.size()) + " VNĐ");
            UI::dong("Lương cao nhất", cao->getHoTen() + " - " + UI::tien(cao->tinhLuong()) + " VNĐ");
        }
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