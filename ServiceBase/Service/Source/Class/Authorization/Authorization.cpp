#pragma once
#include "pch.h"
#include "Authorization.h"

authorize::authorize(std::string _userid, std::string _roleid,std::string _productid, std::string _id)
{
	id = _id;
	userid = _userid;
	roleid = _roleid;
	productid = _productid;
}

tmp_authorize::tmp_authorize(std::string _userid, std::string _roleid, std::string _productid, std::string _buildingid, std::string _status, std::string _id)
{
	id = _id;
	userid = _userid;
	roleid = _roleid;
	productid = _productid;
	buildingid = _buildingid; 
	status = _status;
}
