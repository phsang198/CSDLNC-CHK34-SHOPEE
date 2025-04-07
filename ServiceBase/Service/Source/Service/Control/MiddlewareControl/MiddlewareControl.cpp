#pragma once 
#include "pch.h"
#include "Control/MainControl/IDNService.h" 
#include "ConnectionPool/CrowRequestPool.h"
#include "Rbac/Rbac.h"


SecurityMiddleware::SecurityMiddleware()
{
}
void SecurityMiddleware::setMiddleware(tokenManager TKManager, std::string _productid)
{
	m_token = TKManager;
	productid = _productid;
}
void SecurityMiddleware::after_handle(crow::request& req, crow::response& res, context& ctx)
{
	// no-op
	std::string request_str = "(" + crow::method_name(req.method) + ") (" + req.remote_ip_address + ") " + req.raw_url;

	CrowRequestPool::addConnection(ctx.save, request_str);
}
void SecurityMiddleware::before_handle(crow::request& req, crow::response& res, context& ctx)
{
	int* checkEmpty = CrowRequestPool::getConnection();

	ctx.save = checkEmpty;

	if (!checkEmpty)
	{
		res.code = REQUEST_TIMEOUT;
		return;
	}
	if (method_name(req.method) == "OPTIONS")
	{
		res = RESP();
		return;
	}
	if (/*req.url != "/token/v1/login" && req.url != "/token/v1/relogin" && req.url != "/token/v1/logout" &&*/
		/*req.url != "/token/v1/validate" &&*/ req.url != "/schedule/v1/test" && req.url != "/healthy"
		&& req.url != "/config")
	{
		if (req.headers.find("token") != req.headers.end())
		{
			std::string jwt = req.get_header_value("token");
			int error_code = m_token.checkValidate(jwt, productid);
			if (error_code != 200) res = RESP(error_code);
			else
			{
				int error_code = m_token.CheckAccessToken(jwt);
				if (error_code != 200)
				{
					Poco::Logger::root().information("error token", __FILE__, __LINE__);
					res = RESP(error_code);
				}
				else    // xac thuc thanh cong
				{
					std::string _productid;

					m_token.getListAutho(jwt, ctx.listRoles, _productid);	// kiem tra role co match voi API ko

					if (productid != _productid)
					{
						res = RESP(99);
						res.end();
						return;
					}

					ctx.token = jwt; 

					std::string method = crow::method_name(req.method);
					std::string url = req.url;

					for (auto x : ctx.listRoles)
					{
						std::string roleid = x.second.roleid;

						if (rbacManager::lst_rbac[roleid].count(method + url) != 0)
						{
							ctx.auid = x.first;
							ctx.roleid = x.second.roleid;
							ctx.userid = x.second.userid;

							if (roleid == roleManager::lst_role_name["admin system"]) return;

							return;
						}
					}
					res = RESP(99);
				}
			}
		}
		else
		{
			res = RESP(98);
		}

		res.end();
	}
}