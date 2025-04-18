#pragma once 
#include "pch.h"
#include <iomanip>
#include <sstream>
#include <chrono>

#include "CConfig/Config.h"
#include "Control/MainControl/IDNService.h" 


void CIDNService::API_KhachHang()
{
	//--------------------------------------------------------------------------------------------------------------------------
	CROW_ROUTE(app, "/khachhang").methods("GET"_method)
		([this](const crow::request& req)
			{
				// Lấy tất cả khách hàng

				json result;
				result["status"] = (!cache::lstKhachHang.empty()) ? "OK" : "ERROR";
				result["data"] = json::array();

				for (const auto& [id,kh] : cache::lstKhachHang)
				{
					json item;
					item["MaKH"] = kh.MaKH;
					item["MatKhau"] = kh.MatKhau;
					item["Ten"] = kh.Ten;
					item["Sdt"] = kh.Sdt;
					item["Email"] = kh.Email;
					item["NgayDK"] = kh.NgayDK;
					item["MaPTTT"] = kh.MaPTTT;
					result["data"].push_back(item);
				}

				std::ostringstream oss;
				oss << result;
				crow::response res(oss.str()); 
				response::add_header(res);
				res.set_header("content-type", "application/json");
				return res;
			});

	//--------------------------------------------------------------------------------------------------------------------------
	CROW_ROUTE(app, "/khachhang").methods("POST"_method)
		([this](const crow::request& req)
			{
				auto x = crow::json::load(req.body);
				if (!x) return RESP(150);

				KhachHang kh;
				kh.MatKhau = x["MatKhau"].s();
				kh.Ten = x["Ten"].s();
				kh.Sdt = x["Sdt"].s();
				kh.Email = x["Email"].s();
				kh.NgayDK = x["NgayDK"].s();
				kh.MaPTTT = x["MaPTTT"].i();
				kh.MaKH = rand() % 100 + 1;
				cache::lstKhachHang[kh.MaKH] = kh;
				RESOURCETYPE tmp = kh;
				int error = m_ResourceModel.addResource("khachhang", tmp); 
				return RESP(error);
			});

	//--------------------------------------------------------------------------------------------------------------------------
	CROW_ROUTE(app, "/khachhang").methods("PUT"_method)
		([this](const crow::request& req)
			{
				auto x = crow::json::load(req.body);
				if (!x) return RESP(150);

				KhachHang kh;
				kh.MaKH = x["MaKH"].i();
				kh.MatKhau = x["MatKhau"].s();
				kh.Ten = x["Ten"].s();
				kh.Sdt = x["Sdt"].s();
				kh.Email = x["Email"].s();
				kh.NgayDK = x["NgayDK"].s();
				kh.MaPTTT = x["MaPTTT"].i();
				cache::lstKhachHang[kh.MaKH] = kh;
				RESOURCETYPE tmp = kh;
				int error = m_ResourceModel.updateResource("khachhang", tmp);
				return RESP(200);
			});

	//--------------------------------------------------------------------------------------------------------------------------
	CROW_ROUTE(app, "/khachhang").methods("DELETE"_method)
		([this](const crow::request& req)
			{
				auto x = crow::json::load(req.body);
				if (!x) return RESP(150);

				int maKH = x["MaKH"].i();
				cache::lstKhachHang.erase(maKH); 
				return RESP(200);
			});
}
void CIDNService::API_NguoiBan()
{
	//--------------------------------------------------------------------------------------------------------------------------
	CROW_ROUTE(app, "/nguoiban").methods("GET"_method)
		([this](const crow::request& req)
			{
				json result;
				result["status"] = (!cache::lstNguoiBan.empty()) ? "OK" : "ERROR";
				result["data"] = json::array();

				for (const auto& [id, nb] : cache::lstNguoiBan)
				{
					json item;
					item["MaNB"] = nb.MaNB;
					item["Ten"] = nb.Ten;
					item["MST"] = nb.MST;
					item["Sdt"] = nb.Sdt;
					item["Email"] = nb.Email;
					item["NgayDK"] = nb.NgayDK;
					item["MatKhau"] = nb.MatKhau;
					item["MaDC"] = nb.MaDC;
					result["data"].push_back(item);
				}

				std::ostringstream oss;
				oss << result;
				crow::response res(oss.str());
				response::add_header(res);
				res.set_header("content-type", "application/json");
				return res;
			});

	//--------------------------------------------------------------------------------------------------------------------------
	CROW_ROUTE(app, "/nguoiban").methods("POST"_method)
		([this](const crow::request& req)
			{
				auto x = crow::json::load(req.body);
				if (!x) return RESP(150);

				NguoiBan nb;
				nb.MaNB = rand() % 100 + 1;
				nb.Ten = x["Ten"].s();
				nb.MST = x["MST"].s();
				nb.Sdt = x["Sdt"].s();
				nb.Email = x["Email"].s();
				nb.NgayDK = x["NgayDK"].s();
				nb.MatKhau = x["MatKhau"].s();
				nb.MaDC = x["MaDC"].i();

				cache::lstNguoiBan[nb.MaNB] = nb;
				RESOURCETYPE tmp = nb;
				int error = m_ResourceModel.addResource("nguoiban", tmp);

				return RESP(200);
			});

	//--------------------------------------------------------------------------------------------------------------------------
	CROW_ROUTE(app, "/nguoiban").methods("PUT"_method)
		([this](const crow::request& req)
			{
				auto x = crow::json::load(req.body);
				if (!x) return RESP(150);

				NguoiBan nb;
				nb.MaNB = x["MaNB"].i();
				nb.Ten = x["Ten"].s();
				nb.MST = x["MST"].s();
				nb.Sdt = x["Sdt"].s();
				nb.Email = x["Email"].s();
				nb.NgayDK = x["NgayDK"].s();
				nb.MatKhau = x["MatKhau"].s();
				nb.MaDC = x["MaDC"].i();

				cache::lstNguoiBan[nb.MaNB] = nb;
				RESOURCETYPE tmp = nb;
				int error = m_ResourceModel.updateResource("nguoiban", tmp);
				return RESP(200);
			});

	//--------------------------------------------------------------------------------------------------------------------------
	CROW_ROUTE(app, "/nguoiban").methods("DELETE"_method)
		([this](const crow::request& req)
			{
				auto x = crow::json::load(req.body);
				if (!x) return RESP(150);

				int id = x["MaNB"].i();
				cache::lstNguoiBan.erase(id);
				return RESP(200);
			});
}
void CIDNService::API_SanPham()
{
	// GET
	CROW_ROUTE(app, "/sanpham").methods("GET"_method)
		([this](const crow::request& req)
			{
				json result;
				result["status"] = (!cache::lstSanPham.empty()) ? "OK" : "ERROR";
				result["data"] = json::array();

				for (const auto& [id, sp] : cache::lstSanPham)
				{
					json item;
					item["MaSP"] = sp.MaSP;
					item["Ten"] = sp.Ten;
					item["NgayNhapKho"] = sp.NgayNhapKho;
					item["SoLuong"] = sp.SoLuong;
					item["GiaBan"] = sp.GiaBan;
					item["MaNB"] = sp.MaNB;
					result["data"].push_back(item);
				}

				std::ostringstream oss;
				oss << result;
				crow::response res(oss.str());
				response::add_header(res);
				res.set_header("content-type", "application/json");
				return res;
			});

	// POST
	CROW_ROUTE(app, "/sanpham").methods("POST"_method)
		([this](const crow::request& req)
			{
				auto x = crow::json::load(req.body);
				if (!x) return RESP(150);

				SanPham sp;
				sp.MaSP = rand() % 1000 + 1;
				sp.Ten = x["Ten"].s();
				sp.NgayNhapKho = x["NgayNhapKho"].s();
				sp.SoLuong = x["SoLuong"].i();
				sp.GiaBan = x["GiaBan"].d();
				sp.MaNB = x["MaNB"].i();

				cache::lstSanPham[sp.MaSP] = sp;
				RESOURCETYPE tmp = sp;
				int error = m_ResourceModel.addResource("sanpham", tmp);
				return RESP(200);
			});

	// PUT
	CROW_ROUTE(app, "/sanpham").methods("PUT"_method)
		([this](const crow::request& req)
			{
				auto x = crow::json::load(req.body);
				if (!x) return RESP(150);

				SanPham sp;
				sp.MaSP = x["MaSP"].i();
				sp.Ten = x["Ten"].s();
				sp.NgayNhapKho = x["NgayNhapKho"].s();
				sp.SoLuong = x["SoLuong"].i();
				sp.GiaBan = x["GiaBan"].d();
				sp.MaNB = x["MaNB"].i();

				cache::lstSanPham[sp.MaSP] = sp;
				RESOURCETYPE tmp = sp;
				int error = m_ResourceModel.updateResource("sanpham", tmp);
				return RESP(200);
			});

	// DELETE
	CROW_ROUTE(app, "/sanpham").methods("DELETE"_method)
		([this](const crow::request& req)
			{
				auto x = crow::json::load(req.body);
				if (!x) return RESP(150);

				int id = x["MaSP"].i();
				cache::lstSanPham.erase(id);
				return RESP(200);
			});
}
void CIDNService::API_DonHang()
{
	// GET
	CROW_ROUTE(app, "/donhang").methods("GET"_method)
		([this](const crow::request& req)
			{
				json result;
				result["status"] = (!cache::lstDonHang.empty()) ? "OK" : "ERROR";
				result["data"] = json::array();

				for (const auto& [id, dh] : cache::lstDonHang)
				{
					json item;
					item["MaDH"] = dh.MaDH;
					item["NgayDat"] = dh.NgayDat;
					item["TrangThai"] = dh.TrangThai;
					item["MaDC"] = dh.MaDC;
					item["MaKH"] = dh.MaKH;
					item["MaPTTT"] = dh.MaPTTT;
					item["TrangThaiThanhToan"] = dh.TrangThaiThanhToan;
					item["MaVC"] = dh.MaVC;
					item["MaVCNB"] = dh.MaVCNB;
					item["MaPTVC"] = dh.MaPTVC;
					item["TongTien"] = dh.TongTien;
					result["data"].push_back(item);
				}

				std::ostringstream oss;
				oss << result;
				crow::response res(oss.str());
				response::add_header(res);
				res.set_header("content-type", "application/json");
				return res;
			});

	// POST
	CROW_ROUTE(app, "/donhang").methods("POST"_method)
		([this](const crow::request& req)
			{
				auto x = crow::json::load(req.body);
				if (!x) return RESP(150);

				DonHang dh;
				dh.MaDH = rand() % 1000 + 1;
				dh.NgayDat = x["NgayDat"].s();
				dh.TrangThai = x["TrangThai"].s();
				dh.MaDC = x["MaDC"].i();
				dh.MaKH = x["MaKH"].i();
				dh.MaPTTT = x["MaPTTT"].i();
				dh.TrangThaiThanhToan = x["TrangThaiThanhToan"].s();
				dh.MaVC = x["MaVC"].i();
				dh.MaVCNB = x["MaVCNB"].i();
				dh.MaPTVC = x["MaPTVC"].i();
				dh.TongTien = x["TongTien"].d();

				cache::lstDonHang[dh.MaDH] = dh;
				return RESP(200);
			});

	// PUT
	CROW_ROUTE(app, "/donhang").methods("PUT"_method)
		([this](const crow::request& req)
			{
				auto x = crow::json::load(req.body);
				if (!x) return RESP(150);

				DonHang dh;
				dh.MaDH = x["MaDH"].i();
				dh.NgayDat = x["NgayDat"].s();
				dh.TrangThai = x["TrangThai"].s();
				dh.MaDC = x["MaDC"].i();
				dh.MaKH = x["MaKH"].i();
				dh.MaPTTT = x["MaPTTT"].i();
				dh.TrangThaiThanhToan = x["TrangThaiThanhToan"].s();
				dh.MaVC = x["MaVC"].i();
				dh.MaVCNB = x["MaVCNB"].i();
				dh.MaPTVC = x["MaPTVC"].i();
				dh.TongTien = x["TongTien"].d();

				cache::lstDonHang[dh.MaDH] = dh;
				return RESP(200);
			});

	// DELETE
	CROW_ROUTE(app, "/donhang").methods("DELETE"_method)
		([this](const crow::request& req)
			{
				auto x = crow::json::load(req.body);
				if (!x) return RESP(150);

				int id = x["MaDH"].i();
				cache::lstDonHang.erase(id);
				return RESP(200);
			});
}
void CIDNService::API_ChiTietDonHang()
{
	// GET
	CROW_ROUTE(app, "/chitietdonhang").methods("GET"_method)
		([this](const crow::request& req)
			{
				json result;
				result["status"] = (!cache::lstChiTietDonHang.empty()) ? "OK" : "ERROR";
				result["data"] = json::array();

				for (const auto& [id, ctdh] : cache::lstChiTietDonHang)
				{
					json item;
					item["MaDH"] = ctdh.MaDH;
					item["MaSP"] = ctdh.MaSP;
					item["SoLuong"] = ctdh.SoLuong;
					result["data"].push_back(item);
				}

				std::ostringstream oss;
				oss << result;
				crow::response res(oss.str());
				response::add_header(res);
				res.set_header("content-type", "application/json");
				return res;
			});

	// POST
	CROW_ROUTE(app, "/chitietdonhang").methods("POST"_method)
		([this](const crow::request& req)
			{
				auto x = crow::json::load(req.body);
				if (!x) return RESP(150);

				ChiTietDonHang ctdh;
				ctdh.MaDH = x["MaDH"].i();
				ctdh.MaSP = x["MaSP"].i();
				ctdh.SoLuong = x["SoLuong"].i();

				cache::lstChiTietDonHang[ctdh.MaDH] = ctdh;
				return RESP(200);
			});

	// DELETE
	CROW_ROUTE(app, "/chitietdonhang").methods("DELETE"_method)
		([this](const crow::request& req)
			{
				auto x = crow::json::load(req.body);
				if (!x) return RESP(150);

				int maDH = x["MaDH"].i();
				cache::lstChiTietDonHang.erase(maDH);
				return RESP(200);
			});
}
void CIDNService::API_Voucher()
{
	// GET
	CROW_ROUTE(app, "/voucher").methods("GET"_method)
		([this](const crow::request& req)
			{
				json result;
				result["status"] = (!cache::lstVoucher.empty()) ? "OK" : "ERROR";
				result["data"] = json::array();

				for (const auto& [id, vc] : cache::lstVoucher)
				{
					json item;
					item["MaVC"] = vc.MaVC;
					item["TenVC"] = vc.TenVC;
					item["GiaTri"] = vc.GiaTri;
					item["SoLuong"] = vc.SoLuong;
					item["NgayHetHan"] = vc.NgayHetHan;
					item["NgayKhaDung"] = vc.NgayKhaDung;
					result["data"].push_back(item);
				}

				std::ostringstream oss;
				oss << result;
				crow::response res(oss.str());
				response::add_header(res);
				res.set_header("content-type", "application/json");
				return res;
			});

	// POST
	CROW_ROUTE(app, "/voucher").methods("POST"_method)
		([this](const crow::request& req)
			{
				auto x = crow::json::load(req.body);
				if (!x) return RESP(150);

				Voucher vc;
				vc.MaVC = rand() % 1000 + 1;
				vc.TenVC = x["TenVC"].s();
				vc.GiaTri = x["GiaTri"].d();
				vc.SoLuong = x["SoLuong"].i();
				vc.NgayHetHan = x["NgayHetHan"].s();
				vc.NgayKhaDung = x["NgayKhaDung"].s();

				cache::lstVoucher[vc.MaVC] = vc;
				return RESP(200);
			});

	// DELETE
	CROW_ROUTE(app, "/voucher").methods("DELETE"_method)
		([this](const crow::request& req)
			{
				auto x = crow::json::load(req.body);
				if (!x) return RESP(150);

				int maVC = x["MaVC"].i();
				cache::lstVoucher.erase(maVC);
				return RESP(200);
			});
}
void CIDNService::API_VoucherNB()
{
	// GET
	CROW_ROUTE(app, "/vouchernb").methods("GET"_method)
		([this](const crow::request& req)
			{
				json result;
				result["status"] = (!cache::lstVoucherNB.empty()) ? "OK" : "ERROR";
				result["data"] = json::array();

				for (const auto& [id, vcnb] : cache::lstVoucherNB)
				{
					json item;
					item["MaVC"] = vcnb.MaVC;
					item["MaNB"] = vcnb.MaNB;
					item["TenVC"] = vcnb.TenVC;
					item["GiaTri"] = vcnb.GiaTri;
					item["SoLuong"] = vcnb.SoLuong;
					item["NgayHetHan"] = vcnb.NgayHetHan;
					result["data"].push_back(item);
				}

				std::ostringstream oss;
				oss << result;
				crow::response res(oss.str());
				response::add_header(res);
				res.set_header("content-type", "application/json");
				return res;
			});

	// DELETE
	CROW_ROUTE(app, "/vouchernb").methods("DELETE"_method)
		([this](const crow::request& req)
			{
				auto x = crow::json::load(req.body);
				if (!x) return RESP(150);

				int maVC = x["MaVC"].i();
				cache::lstVoucherNB.erase(maVC);
				return RESP(200);
			});
}
void CIDNService::API_ViVoucher()
{
	// GET
	CROW_ROUTE(app, "/vivoucher").methods("GET"_method)
		([this](const crow::request& req)
			{
				json result;
				result["status"] = (!cache::lstViVoucher.empty()) ? "OK" : "ERROR";
				result["data"] = json::array();

				for (const auto& [id, vv] : cache::lstViVoucher)
				{
					json item;
					item["MaVi"] = vv.MaVi;
					item["MaKH"] = vv.MaKH;
					item["MaVC"] = vv.MaVC;
					result["data"].push_back(item);
				}

				std::ostringstream oss;
				oss << result;
				crow::response res(oss.str());
				response::add_header(res);
				res.set_header("content-type", "application/json");
				return res;
			});

	// DELETE
	CROW_ROUTE(app, "/vivoucher").methods("DELETE"_method)
		([this](const crow::request& req)
			{
				auto x = crow::json::load(req.body);
				if (!x) return RESP(150);

				int maVi = x["MaVi"].i();
				cache::lstViVoucher.erase(maVi);
				return RESP(200);
			});
}
