#pragma once

#include "Resource/Resource.h"

class OGRFeature;

typedef std::variant<resource> RESOURCETYPE;


class ResourceModel
{
public:
	ResourceModel();
	~ResourceModel();
public:
	int addResource(resource& _Resource);

	int getResource(std::string rscname, std::string auid, std::vector<RESOURCETYPE>& lst_Resource);

	int deleteResource(const std::string rscname, const std::string& auid, const std::string& id = "");
	int Query(const std::string& id, const std::string& geos, const std::string& tablename = "geom_data");
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