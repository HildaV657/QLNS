// Chương trình chính - Quản lý nhân sự (C++ OOP, console)
// Biên dịch: g++ -std=c++17 main.cpp -o main.exe
// Lưu tất cả file ở dạng UTF-8. Nên chạy trong Windows Terminal.
#include "ConsoleUI.cpp"
#include "DanhSachNhanVien.cpp"
using namespace std;

template <class T>
void themNhanVien(DanhSachNhanVien& ql, const string& tieuDe)
{
    UI::dangNhapNhanVien = true;
    UI::huyNhapNhanVien = false;
    UI::xoaManHinh();
    cout << "\n  " << UI::VANG << "** " << tieuDe << " **" << UI::RESET << "\n\n";
    cout << UI::XAM << "Nhấn ESC bất cứ lúc nào để hủy thao tác." << UI::RESET << "\n\n";
    string ma;
    while (true)
    {
        ma = UI::docChuoi("Mã nhân viên: ");
        if (UI::huyNhapNhanVien) break;
        if (!ql.timTheoMa(ma)) break;
        cout << UI::DO << "  ! Mã đã tồn tại, nhập mã khác.\n" << UI::RESET;
    }
    if (UI::huyNhapNhanVien)
    {
        UI::dangNhapNhanVien = false;
        UI::huyNhapNhanVien = false;
        cout << UI::VANG << "\n  Đã hủy thao tác thêm nhân viên." << UI::RESET << "\n";
        UI::tamDung();
        return;
    }
    T* p = new T();
    p->setMaNV(ma);
    cin >> *p;
    if (UI::huyNhapNhanVien)
    {
        delete p;
        UI::dangNhapNhanVien = false;
        UI::huyNhapNhanVien = false;
        cout << UI::VANG << "\n  Đã hủy thao tác thêm nhân viên." << UI::RESET << "\n";
        UI::tamDung();
        return;
    }
    ql.them(p);
    cout << UI::XANH << "\n  ✓ Đã thêm: " << *p << UI::RESET << "\n";
    UI::dangNhapNhanVien = false;
    UI::huyNhapNhanVien = false;
    UI::tamDung();
}

const string FILE_MAC_DINH = "danhsach_nhanvien.txt";

// Hỏi tên file (Enter = tên mặc định, ESC = hủy -> trả về chuỗi rỗng)
string hoiTenFile(const string& tieuDe)
{
    cout << "\n  " << UI::VANG << "** " << tieuDe << " **" << UI::RESET << "\n";
    cout << UI::XAM << "  Enter để dùng \"" << FILE_MAC_DINH << "\", ESC để hủy.\n" << UI::RESET;
    string s;
    if (!UI::docDongCoEsc("  Tên file: ", s)) return "";
    s = UI::cat(s);
    return s.empty() ? FILE_MAC_DINH : s;
}

int main() 
{
    UI::khoiTao();
    DanhSachNhanVien ql;

    UI::xoaManHinh();
    cout << "\n  " << UI::VANG << "** QUẢN LÝ NHÂN SỰ **" << UI::RESET << "\n\n";
    ql.setThang((int)UI::docSo("Nhập tháng tính lương (1-12): ", 1, 12));

    vector<string> menu =
    {
        "Thêm nhân viên văn phòng",
        "Thêm nhân viên sản xuất",
        "Thêm quản lý",
        "Hiển thị danh sách nhân viên",
        "Xem chi tiết 1 nhân viên",
        "Tìm nhân viên theo từ khóa",
        "Sắp xếp theo lương giảm dần",
        "Xóa nhân viên",
        "Xem lịch sử xóa nhân viên",
        "Thống kê lương",
        "Đổi tháng tính lương",
        "Nạp dữ liệu mẫu",
        "Lưu danh sách ra file txt",
        "Nạp dữ liệu từ file txt",
        "Thoát"
    };

    while (true) 
    {
        string phu = "Tháng " + to_string(ql.getThang()) + " | Số nhân viên: " + to_string(ql.soLuong());
        int c = UI::chonMenu("QUẢN LÝ NHÂN SỰ", phu, menu);
        UI::xoaManHinh();

        if (c == 0) themNhanVien<NhanVienVanPhong>(ql, "THÊM NHÂN VIÊN VĂN PHÒNG");
        else if (c == 1) themNhanVien<NhanVienSanXuat>(ql, "THÊM NHÂN VIÊN SẢN XUẤT");
        else if (c == 2) themNhanVien<QuanLy>(ql, "THÊM QUẢN LÝ");
        else if (c == 3) 
        {
            ql.inTatCa();
            UI::tamDung();
        } 
        else if (c == 4) 
        {
            string ma = UI::docChuoi("\n  Nhập mã nhân viên: ");
            NhanVien* p = ql.timTheoMa(ma);
            if (!p) cout << UI::DO << "\n  ! Không tìm thấy nhân viên.\n" << UI::RESET;
            else 
            {
                cout << "\n  " << UI::VANG << "** THÔNG TIN NHÂN VIÊN **" << UI::RESET << "\n\n";
                p->hienThiThongTin(ql.getThang()); // đa hình
            }
            UI::tamDung();
        } 
        else if (c == 5) 
        {
            string tk = UI::docChuoi("\n  Tìm tất cả nhân viên liên quan đến từ khóa: ");
            auto kq = ql.timTheoTuKhoa(tk);
            if (kq.empty()) cout << UI::DO << "\n  ! Không có kết quả.\n" << UI::RESET;
            else ql.inBang(kq, "KẾT QUẢ TÌM KIẾM");
            UI::tamDung();
        } 
        else if (c == 6) 
        {
            ql.sapXepLuongGiam();
            ql.inTatCa();
            UI::tamDung();
        } 
        else if (c == 7) 
        {
            string ma = UI::docChuoi("\n  Nhập mã nhân viên cần xóa: ");
            string LyDo = UI::docChuoi("\n  Nhập lý do xóa nhân viên: ");
            if (ql.xoa(ma, LyDo)) 
            {
                cout << UI::XANH << "\n  ✓ Đã xóa.\n"  << "\n Lý Do: " << LyDo  << "\n" << UI::RESET;
            }
            else cout << UI::DO << "\n  ! Không tìm thấy nhân viên.\n" << UI::RESET;
            UI::tamDung();
        } 
        
        else if (c == 8)
        {
            ql.xemLichSuXoa();
            UI::tamDung();
        }
        else if (c == 9) 
        {
            ql.thongKe();
            UI::tamDung();
        } 
        else if (c == 10) 
        {
            ql.setThang((int)UI::docSo("\n  Nhập tháng mới (1-12): ", 1, 12));
        } 
        else if (c == 11) 
        {
            if (ql.timTheoMa("VP01")) cout << UI::DO << "\n  ! Dữ liệu mẫu đã được nạp rồi.\n" << UI::RESET;
            else 
            {
                ql.napMau();
                cout << UI::XANH << "\n  ✓ Đã nạp 5 nhân viên mẫu.\n" << UI::RESET;
            }
            UI::tamDung();
        } 
        else if (c == 12)
        {
            string f = hoiTenFile("Lưu ra file");
            if (!f.empty())
            {
                if (ql.luuFile(f)) cout << UI::XANH << "\n  ✓ Đã lưu " << ql.soLuong() << " nhân viên vào \"" << f << "\".\n" << UI::RESET;
                else cout << UI::DO << "\n  ! Không thể ghi file \"" << f << "\".\n" << UI::RESET;
            }
            UI::tamDung();
        }
        else if (c == 13)
        {
            string f = hoiTenFile("Nạp từ file");
            if (!f.empty())
            {
                int them = 0, trung = 0, loi = 0;
                if (!ql.docFile(f, them, trung, loi))
                    cout << UI::DO << "\n  ! Không mở được file \"" << f << "\" (chưa có file? hãy lưu trước).\n" << UI::RESET;
                else
                {
                    cout << UI::XANH << "\n  ✓ Đã nạp " << them << " nhân viên từ \"" << f << "\".\n" << UI::RESET;
                    if (trung) cout << UI::VANG << "  ! Bỏ qua " << trung << " nhân viên trùng mã.\n" << UI::RESET;
                    if (loi) cout << UI::DO << "  ! Bỏ qua " << loi << " dòng sai định dạng.\n" << UI::RESET;
                }
            }
            UI::tamDung();
        }
        else break;
    }
    UI::xoaManHinh();
    cout << "Tạm biệt!\n";
    return 0;
}
