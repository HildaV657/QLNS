#pragma once
#include <sstream>
#include "NhanVien.cpp"
using namespace std;

class NhanVienVanPhong : public NhanVien 
{
    int soNgayLamViec;
    int soNgayNghi; // 1 tháng chỉ được nghỉ 3 ngày, k tính t7, cn, nếu nghỉ nhiều hơn bị trừ lương 50k/1 ngày 
    double donGiaNgay;
    double soGioOT;
    double heSoOT; // OT nhân lên (mặc định 1.5)
    static const int GIO_CHUAN = 8; // 1 ngày công = 8 giờ
public:
    NhanVienVanPhong(const string& ma = "", const string& ten = "", const Ngay& ns = Ngay(), const string& dc = "",
                     int ngay = 0,int soNgayNghi = 0 ,double donGia = 0, double ot = 0, double heSo = 1.5)
        : NhanVien(ma, ten, ns, dc), soNgayLamViec(ngay), donGiaNgay(donGia), soGioOT(ot), heSoOT(heSo), soNgayNghi(soNgayNghi) {}
    NhanVienVanPhong(const NhanVienVanPhong& o)
        : NhanVien(o), soNgayLamViec(o.soNgayLamViec), donGiaNgay(o.donGiaNgay), soGioOT(o.soGioOT), heSoOT(o.heSoOT), soNgayNghi (o.soNgayNghi) {}
    ~NhanVienVanPhong() {}

    int getSoNgayLamViec() const { return soNgayLamViec; }
    int getsoNgayNghi() const { return soNgayNghi; }
    double getDonGiaNgay() const { return donGiaNgay; }
    double getSoGioOT() const { return soGioOT; }
    double getHeSoOT() const { return heSoOT; }
    void setSoNgayLamViec(int x) { soNgayLamViec = x; }
    void setsoNgayNghi (int x) { soNgayNghi = x; }
    void setDonGiaNgay(double x) { donGiaNgay = x; }
    void setSoGioOT(double x) { soGioOT = x; }
    void setHeSoOT(double x) { heSoOT = x; }

    double luongCung() const { return soNgayLamViec * donGiaNgay; }
    double luongOT() const { return soGioOT * (donGiaNgay / GIO_CHUAN) * heSoOT; }
    double tienPhatNghi() const { return max(0, soNgayNghi - 3) * 50000.0;}

    double tinhLuong() const override { return luongCung() + luongOT() - tienPhatNghi (); }
    string loaiNV() const override { return "Văn phòng"; }
    NhanVien* clone() const override { return new NhanVienVanPhong(*this); }
    string ghiFile() const override
    {
        return ghiChung("VP") + "\t" + to_string(soNgayLamViec) + "\t" + so(donGiaNgay) + "\t" + so(soGioOT) + "\t" + so(heSoOT);
    }

    void nhap() override
    {
        NhanVien::nhap();
        if (UI::huyNhapNhanVien) return;
        soNgayLamViec = (int)UI::docSo("Số ngày làm việc (0-31): ", 0, 31);
        soNgayNghi = (int)UI::docSo("Số ngày nghỉ trong tháng: ", 0, 31);
        if (UI::huyNhapNhanVien) return;
        donGiaNgay = UI::docSo("Đơn giá ngày (VNĐ): ", 0);
        if (UI::huyNhapNhanVien) return;
        soGioOT = UI::docSo("Số giờ OT: ", 0);
        if (UI::huyNhapNhanVien) return;
        heSoOT = UI::docSo("Hệ số OT (vd 1.5): ", 1);
        if (UI::huyNhapNhanVien) return;
    }

    void hienThiThongTin(int thang) const override 
    {
        NhanVien::hienThiThongTin(thang);
        UI::dong("Số ngày làm việc", to_string(soNgayLamViec));
        UI::dong("Đơn giá ngày", UI::tien(donGiaNgay) + " VNĐ");
        UI::dong("Lương cứng", UI::tien(luongCung()) + " VNĐ");
        ostringstream os;
        os << soGioOT << " giờ x " << heSoOT << " = " << UI::tien(luongOT()) << " VNĐ";
        UI::dong("Tiền OT", os.str());
        UI::dong("Số ngày nghỉ", to_string(soNgayNghi));
        UI::dong("Phạt nghỉ vượt phép", UI::tien(tienPhatNghi()) + " VNĐ");
        inLuong(thang);
    }
};
