#pragma once 
#include "pch.h"
#include "ResourceModel.h"
#include "ConnectionPool/ConnectionPool.h"


std::map<int, KhachHang> cache::lstKhachHang;
std::map<int, NguoiBan> cache::lstNguoiBan;
std::map<int, DiaChi> cache::lstDiaChi;
std::map<int, PhuongThucThanhToan> cache::lstPhuongThucThanhToan;
std::map<int, PhuongThucVanChuyen> cache::lstPhuongThucVanChuyen;
std::map<int, Voucher> cache::lstVoucher;
std::map<int, VoucherNB> cache::lstVoucherNB;
std::map<int, ViVoucher> cache::lstViVoucher;
std::map<int, SanPham> cache::lstSanPham;
std::map<int, DonHang> cache::lstDonHang;
std::map<int, ChiTietDonHang> cache::lstChiTietDonHang;
std::map<int, ChiTietTT> cache::lstChiTietTT;
std::map<int, PTVCNB> cache::lstPTVCNB;

ResourceModel::ResourceModel()
{
}

ResourceModel::~ResourceModel()
{
}

int ResourceModel::addResource(std::string rscname, RESOURCETYPE& _Resource)
{
	std::string query;
	std::string idColumn;
	std::string id;

	if (rscname == "khachhang") {
		auto tmp = std::get<KhachHang>(_Resource);
		query = R"(INSERT INTO khachhang(matkhau, ten, sdt, email, ngaydk, mapttt)
		           VALUES ('$matkhau', '$ten', '$sdt', '$email', '$ngaydk', $mapttt)
		           RETURNING makh)";
		idColumn = "makh";
		StringProcess::ReplaceAll(query, "$matkhau", tmp.MatKhau);
		StringProcess::ReplaceAll(query, "$ten", tmp.Ten);
		StringProcess::ReplaceAll(query, "$sdt", tmp.Sdt);
		StringProcess::ReplaceAll(query, "$email", tmp.Email);
		StringProcess::ReplaceAll(query, "$ngaydk", tmp.NgayDK);
		StringProcess::ReplaceAll(query, "$mapttt", std::to_string(tmp.MaPTTT));
	}
	else if (rscname == "nguoiban") {
		auto tmp = std::get<NguoiBan>(_Resource);
		query = R"(INSERT INTO nguoiban(ten, mst, sdt, email, ngaydk, matkhau, madc)
		           VALUES ('$ten', '$mst', '$sdt', '$email', '$ngaydk', '$matkhau', $madc)
		           RETURNING manb)";
		idColumn = "manb";
		StringProcess::ReplaceAll(query, "$ten", tmp.Ten);
		StringProcess::ReplaceAll(query, "$mst", tmp.MST);
		StringProcess::ReplaceAll(query, "$sdt", tmp.Sdt);
		StringProcess::ReplaceAll(query, "$email", tmp.Email);
		StringProcess::ReplaceAll(query, "$ngaydk", tmp.NgayDK);
		StringProcess::ReplaceAll(query, "$matkhau", tmp.MatKhau);
		StringProcess::ReplaceAll(query, "$madc", std::to_string(tmp.MaDC));
	}
	else if (rscname == "sanpham") {
		auto tmp = std::get<SanPham>(_Resource);
		query = R"(INSERT INTO sanpham(ten, ngaynhapkho, soluong, giaban, manb)
		           VALUES ('$ten', '$ngaynhapkho', $soluong, $giaban, $manb)
		           RETURNING masp)";
		idColumn = "masp";
		StringProcess::ReplaceAll(query, "$ten", tmp.Ten);
		StringProcess::ReplaceAll(query, "$ngaynhapkho", tmp.NgayNhapKho);
		StringProcess::ReplaceAll(query, "$soluong", std::to_string(tmp.SoLuong));
		StringProcess::ReplaceAll(query, "$giaban", std::to_string(tmp.GiaBan));
		StringProcess::ReplaceAll(query, "$manb", std::to_string(tmp.MaNB));
	}
	else if (rscname == "donhang") {
		auto tmp = std::get<DonHang>(_Resource);
		query = R"(INSERT INTO donhang(ngaydat, trangthai, madc, makh, mapttt, trangthaithanhtoan, mavc, mavcnb, maptvc, tongtien)
		           VALUES ('$ngaydat', '$trangthai', $madc, $makh, $mapttt, '$trangthaithanhtoan', $mavc, $mavcnb, $maptvc, $tongtien)
		           RETURNING madh)";
		idColumn = "madh";
		StringProcess::ReplaceAll(query, "$ngaydat", tmp.NgayDat);
		StringProcess::ReplaceAll(query, "$trangthai", tmp.TrangThai);
		StringProcess::ReplaceAll(query, "$madc", std::to_string(tmp.MaDC));
		StringProcess::ReplaceAll(query, "$makh", std::to_string(tmp.MaKH));
		StringProcess::ReplaceAll(query, "$mapttt", std::to_string(tmp.MaPTTT));
		StringProcess::ReplaceAll(query, "$trangthaithanhtoan", tmp.TrangThaiThanhToan);
		StringProcess::ReplaceAll(query, "$mavc", std::to_string(tmp.MaVC));
		StringProcess::ReplaceAll(query, "$mavcnb", std::to_string(tmp.MaVCNB));
		StringProcess::ReplaceAll(query, "$maptvc", std::to_string(tmp.MaPTVC));
		StringProcess::ReplaceAll(query, "$tongtien", std::to_string(tmp.TongTien));
	}
	else if (rscname == "voucher") {
		auto tmp = std::get<Voucher>(_Resource);
		query = R"(INSERT INTO voucher(tenvc, giatri, soluong, ngayhethan, ngaykhadung)
		           VALUES ('$tenvc', $giatri, $soluong, '$ngayhethan', '$ngaykhadung')
		           RETURNING mavc)";
		idColumn = "mavc";
		StringProcess::ReplaceAll(query, "$tenvc", tmp.TenVC);
		StringProcess::ReplaceAll(query, "$giatri", std::to_string(tmp.GiaTri));
		StringProcess::ReplaceAll(query, "$soluong", std::to_string(tmp.SoLuong));
		StringProcess::ReplaceAll(query, "$ngayhethan", tmp.NgayHetHan);
		StringProcess::ReplaceAll(query, "$ngaykhadung", tmp.NgayKhaDung);
	}
	else if (rscname == "diachi") {
		auto tmp = std::get<DiaChi>(_Resource);
		query = R"(INSERT INTO diachi(tendc) VALUES ('$tendc') RETURNING madc)";
		idColumn = "madc";
		StringProcess::ReplaceAll(query, "$tendc", tmp.TenDC);
	}
	else {
		return 404;
	}

	return ConnectionManager::QueryOrther(query, id, idColumn);
}

int ResourceModel::updateResource(std::string rscname, RESOURCETYPE& _Resource)
{
	std::string query;
	std::string idColumn;
	std::string id;

	if (rscname == "khachhang") {
		auto tmp = std::get<KhachHang>(_Resource);
		query = R"(UPDATE khachhang SET matkhau = '$matkhau', ten = '$ten', sdt = '$sdt', email = '$email', ngaydk = '$ngaydk', mapttt = $mapttt WHERE makh = $makh RETURNING makh)";
		idColumn = "makh";
		StringProcess::ReplaceAll(query, "$makh", std::to_string(tmp.MaKH));
		StringProcess::ReplaceAll(query, "$matkhau", tmp.MatKhau);
		StringProcess::ReplaceAll(query, "$ten", tmp.Ten);
		StringProcess::ReplaceAll(query, "$sdt", tmp.Sdt);
		StringProcess::ReplaceAll(query, "$email", tmp.Email);
		StringProcess::ReplaceAll(query, "$ngaydk", tmp.NgayDK);
		StringProcess::ReplaceAll(query, "$mapttt", std::to_string(tmp.MaPTTT));
	}
	else if (rscname == "nguoiban") {
		auto tmp = std::get<NguoiBan>(_Resource);
		query = R"(UPDATE nguoiban SET ten = '$ten', mst = '$mst', sdt = '$sdt', email = '$email', ngaydk = '$ngaydk', matkhau = '$matkhau', madc = $madc WHERE manb = $manb RETURNING manb)";
		idColumn = "manb";
		StringProcess::ReplaceAll(query, "$manb", std::to_string(tmp.MaNB));
		StringProcess::ReplaceAll(query, "$ten", tmp.Ten);
		StringProcess::ReplaceAll(query, "$mst", tmp.MST);
		StringProcess::ReplaceAll(query, "$sdt", tmp.Sdt);
		StringProcess::ReplaceAll(query, "$email", tmp.Email);
		StringProcess::ReplaceAll(query, "$ngaydk", tmp.NgayDK);
		StringProcess::ReplaceAll(query, "$matkhau", tmp.MatKhau);
		StringProcess::ReplaceAll(query, "$madc", std::to_string(tmp.MaDC));
	}
	else if (rscname == "sanpham") {
		auto tmp = std::get<SanPham>(_Resource);
		query = R"(UPDATE sanpham SET ten = '$ten', ngaynhapkho = '$ngaynhapkho', soluong = $soluong, giaban = $giaban, manb = $manb WHERE masp = $masp RETURNING masp)";
		idColumn = "masp";
		StringProcess::ReplaceAll(query, "$masp", std::to_string(tmp.MaSP));
		StringProcess::ReplaceAll(query, "$ten", tmp.Ten);
		StringProcess::ReplaceAll(query, "$ngaynhapkho", tmp.NgayNhapKho);
		StringProcess::ReplaceAll(query, "$soluong", std::to_string(tmp.SoLuong));
		StringProcess::ReplaceAll(query, "$giaban", std::to_string(tmp.GiaBan));
		StringProcess::ReplaceAll(query, "$manb", std::to_string(tmp.MaNB));
	}
	else if (rscname == "donhang") {
		auto tmp = std::get<DonHang>(_Resource);
		query = R"(UPDATE donhang SET ngaydat = '$ngaydat', trangthai = '$trangthai', madc = $madc, makh = $makh, mapttt = $mapttt, trangthaithanhtoan = '$trangthaithanhtoan', mavc = $mavc, mavcnb = $mavcnb, maptvc = $maptvc, tongtien = $tongtien WHERE madh = $madh RETURNING madh)";
		idColumn = "madh";
		StringProcess::ReplaceAll(query, "$madh", std::to_string(tmp.MaDH));
		StringProcess::ReplaceAll(query, "$ngaydat", tmp.NgayDat);
		StringProcess::ReplaceAll(query, "$trangthai", tmp.TrangThai);
		StringProcess::ReplaceAll(query, "$madc", std::to_string(tmp.MaDC));
		StringProcess::ReplaceAll(query, "$makh", std::to_string(tmp.MaKH));
		StringProcess::ReplaceAll(query, "$mapttt", std::to_string(tmp.MaPTTT));
		StringProcess::ReplaceAll(query, "$trangthaithanhtoan", tmp.TrangThaiThanhToan);
		StringProcess::ReplaceAll(query, "$mavc", std::to_string(tmp.MaVC));
		StringProcess::ReplaceAll(query, "$mavcnb", std::to_string(tmp.MaVCNB));
		StringProcess::ReplaceAll(query, "$maptvc", std::to_string(tmp.MaPTVC));
		StringProcess::ReplaceAll(query, "$tongtien", std::to_string(tmp.TongTien));
	}
	else if (rscname == "voucher") {
		auto tmp = std::get<Voucher>(_Resource);
		query = R"(UPDATE voucher SET tenvc = '$tenvc', giatri = $giatri, soluong = $soluong, ngayhethan = '$ngayhethan', ngaykhadung = '$ngaykhadung' WHERE mavc = $mavc RETURNING mavc)";
		idColumn = "mavc";
		StringProcess::ReplaceAll(query, "$mavc", std::to_string(tmp.MaVC));
		StringProcess::ReplaceAll(query, "$tenvc", tmp.TenVC);
		StringProcess::ReplaceAll(query, "$giatri", std::to_string(tmp.GiaTri));
		StringProcess::ReplaceAll(query, "$soluong", std::to_string(tmp.SoLuong));
		StringProcess::ReplaceAll(query, "$ngayhethan", tmp.NgayHetHan);
		StringProcess::ReplaceAll(query, "$ngaykhadung", tmp.NgayKhaDung);
	}
	else if (rscname == "diachi") {
		auto tmp = std::get<DiaChi>(_Resource);
		query = R"(UPDATE diachi SET tendc = '$tendc' WHERE madc = $madc RETURNING madc)";
		idColumn = "madc";
		StringProcess::ReplaceAll(query, "$madc", std::to_string(tmp.MaDC));
		StringProcess::ReplaceAll(query, "$tendc", tmp.TenDC);
	}
	return ConnectionManager::QueryOrther(query, id, idColumn);
}
int ResourceModel::getResource(std::string rscname,  std::vector<RESOURCETYPE>& lst_Resource)
{
	std::string query = R"(SELECT * FROM "$rscname" )";


	StringProcess::ReplaceAll(query, "$rscname", rscname);

	GDALDataset* geDS;
	OGRLayer* poLayer;
	OGRFeature* poFeature;

	int test = ConnectionManager::QueryGet(geDS, poLayer, query);
	if (test == 200)
	{
		while ((poFeature = poLayer->GetNextFeature()) != NULL)
		{
		}
		geDS->ReleaseResultSet(poLayer);
	}
	CConnectionPool::addConnection(geDS);
	return test;
}

int ResourceModel::deleteResource(const std::string rscname , const std::string& id)
{
	std::string query =
		query = R"(DELETE FROM "$rscname"
					WHERE "$rscname".id = '$id'
					) 
				)";
	StringProcess::ReplaceAll(query, "$id", id);
	StringProcess::ReplaceAll(query, "$rscname", rscname);

	std::string tmpid;
	return ConnectionManager::QueryOrther(query, tmpid);
}

