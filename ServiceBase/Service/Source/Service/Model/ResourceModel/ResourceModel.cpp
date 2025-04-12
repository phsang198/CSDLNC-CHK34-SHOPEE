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

int ResourceModel::addResource(resource& _Resource)
{
	std::string query =
		R"(INSERT INTO "resourcemanager"(auid,rscid,rscname,belongname,belongid) 
			VALUES ('$auid','$rscid','$rscname','$belongname','$belongid')
			RETURNING id )";

	StringProcess::ReplaceAll(query, "$auid", _Resource.auid);
	StringProcess::ReplaceAll(query, "$rscid", _Resource.rscid);
	StringProcess::ReplaceAll(query, "$rscname", _Resource.rscname);
	if (_Resource.belongid != "")
		StringProcess::ReplaceAll(query, "$belongname", _Resource.belongname);
	else
		StringProcess::ReplaceAll(query, R"('$belongname')", "NULL");
	if (_Resource.belongid != "")
		StringProcess::ReplaceAll(query, "$belongid", _Resource.belongid);
	else
		StringProcess::ReplaceAll(query, R"('$belongid')", "NULL");

	return ConnectionManager::QueryOrther(query, _Resource.id);
}


int ResourceModel::getResource(std::string rscname, std::string auid, std::vector<RESOURCETYPE>& lst_Resource)
{
	std::string query = R"(SELECT * FROM "resourcemanager" 
						   WHERE  rscname = '$rscname' AND auid = '$auid' )";


	StringProcess::ReplaceAll(query, "$auid", auid);
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
int ResourceModel::deleteResource(const std::string rscname , const std::string& auid, const std::string& id)
{
	std::string query =
		R"(DELETE from "Resource"
			WHERE "Resource".id = '$id' 
			RETURNING id )";

	if (rscname != "resource")
	{
		query = R"(DELETE FROM "$rscname"
					WHERE id IN (
						SELECT rscid
						FROM resourcemanager
						WHERE auid = '$auid' AND rscname = '$rscname'
					) 
				)";
		if (id != "") query += "AND id = '$id'"; 
		query += " RETURNING id ";
	}
	StringProcess::ReplaceAll(query, "$id", id);
	StringProcess::ReplaceAll(query, "$auid", auid);
	StringProcess::ReplaceAll(query, "$rscname", rscname);

	std::string tmpid;
	return ConnectionManager::QueryOrther(query, tmpid);
}

int ResourceModel::Query(const std::string& id, const std::string& geos, const std::string& tablename)
{
	std::string query =
		R"(
			CREATE TABLE IF NOT EXISTS public."geom_data"
			(
				id uuid NOT NULL DEFAULT uuid_generate_v4(),
				geom GEOMETRY 
			))";

	std::string tmpid;
	ConnectionManager::QueryOrther(query, tmpid);

	query = R"(INSERT INTO geom_data (id, geom) 
			VALUES ('$id',ST_GeomFromWKB('$wkb', 4326)) )";
	StringProcess::ReplaceAll(query, "$id", id);
	StringProcess::ReplaceAll(query, "$wkb", geos);

	return ConnectionManager::QueryOrther(query, tmpid);

}

