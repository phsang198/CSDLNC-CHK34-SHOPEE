#pragma once
#include <string>
#include <vector>
#include <map>

// Lớp resource đã cho
class resource {
public:
    resource();
    resource(std::string _auid, std::string _rscname, std::string _rscid,
        std::string _belongname = "", std::string _belongid = "", std::string _id = "");
    ~resource() {}
public:
    std::string id;
    std::string auid;
    std::string rscname;
    std::string rscid;
    std::string belongname;
    std::string belongid;
};

class DiaChi {
public:
    int MaDC;
    std::string TenDC;
};

class NguoiBan {
public:
    int MaNB;
    std::string Ten;
    std::string MST;
    std::string Sdt;
    std::string Email;
    std::string NgayDK;
    std::string MatKhau;
    int MaDC;
};

class PhuongThucThanhToan {
public:
    int MaPTTT;
    std::string Ten;
    std::string Loai;
};

class KhachHang {
public:
    int MaKH;
    std::string MatKhau;
    std::string Ten;
    std::string Sdt;
    std::string Email;
    std::string NgayDK;
    int MaPTTT;
};

class ChiTietTT {
public:
    int MaKH;
    int MaPTTT;
};

class PhuongThucVanChuyen {
public:
    int MaPTVC;
    std::string TenPTVC;
    double Gia;
};

class PTVCNB {
public:
    int MaNB;
    int MaPTVC;
};

class Voucher {
public:
    int MaVC;
    std::string TenVC;
    double GiaTri;
    int SoLuong;
    std::string NgayHetHan;
    std::string NgayKhaDung;
};

class VoucherNB {
public:
    int MaVC;
    int MaNB;
    std::string TenVC;
    double GiaTri;
    int SoLuong;
    std::string NgayHetHan;
};

class ViVoucher {
public:
    int MaVi;
    int MaKH;
    int MaVC;
};

class SanPham {
public:
    int MaSP;
    std::string Ten;
    std::string NgayNhapKho;
    int SoLuong;
    double GiaBan;
    int MaNB;
};

class DonHang {
public:
    int MaDH;
    std::string NgayDat;
    std::string TrangThai;
    int MaDC;
    int MaKH;
    int MaPTTT;
    std::string TrangThaiThanhToan;
    int MaVC;
    int MaVCNB;
    int MaPTVC;
    double TongTien;
};

class ChiTietDonHang {
public:
    int MaDH;
    int MaSP;
    int SoLuong;
};

class resourceManager
{
public:
	static std::map<std::string, resource> lst_resource; // first : resourceid , second : detail 
	static std::map<std::string, std::string> lst_resource_name; // first : resource name , second : resourceid 
public:
	static void id2name();
};
