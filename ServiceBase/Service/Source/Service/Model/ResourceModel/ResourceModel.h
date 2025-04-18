#pragma once

#include "Resource/Resource.h"

class OGRFeature;

typedef std::variant<KhachHang,DiaChi,NguoiBan, PTVCNB, SanPham, DonHang, ChiTietDonHang, ViVoucher, VoucherNB, Voucher, PhuongThucVanChuyen,PhuongThucThanhToan, ChiTietTT,resource> RESOURCETYPE;


class ResourceModel
{
public:
	ResourceModel();
	~ResourceModel();
public:
	int addResource(std::string rscname, RESOURCETYPE& _Resource);

	int updateResource(std::string rscname, RESOURCETYPE& _Resource);

	int getResource(std::string rscname, std::vector<RESOURCETYPE>& lst_Resource);

	int deleteResource(const std::string rscname, const std::string& id = "");
};

class cache
{
public:
	static std::map<int, KhachHang> lstKhachHang;
	static std::map<int, NguoiBan> lstNguoiBan;
	static std::map<int, DiaChi> lstDiaChi;
	static std::map<int, PhuongThucThanhToan> lstPhuongThucThanhToan;
	static std::map<int, PhuongThucVanChuyen> lstPhuongThucVanChuyen;
	static std::map<int, Voucher> lstVoucher;
	static std::map<int, VoucherNB> lstVoucherNB;
	static std::map<int, ViVoucher> lstViVoucher;
	static std::map<int, SanPham> lstSanPham;
	static std::map<int, DonHang> lstDonHang;
	static std::map<int, ChiTietDonHang> lstChiTietDonHang;
	static std::map<int, ChiTietTT> lstChiTietTT;
	static std::map<int, PTVCNB> lstPTVCNB;
};