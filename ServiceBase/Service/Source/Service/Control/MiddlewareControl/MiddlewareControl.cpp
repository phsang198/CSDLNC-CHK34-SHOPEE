#pragma once 
#include "pch.h"
#include "Control/MainControl/IDNService.h" 
#include "ConnectionPool/CrowRequestPool.h"


SecurityMiddleware::SecurityMiddleware()
{
}
void SecurityMiddleware::setMiddleware()
{
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
}