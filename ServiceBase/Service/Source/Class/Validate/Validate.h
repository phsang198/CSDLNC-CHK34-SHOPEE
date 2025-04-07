#pragma once
#include <pch.h>

namespace validate
{
	class header
	{
	public:
		static void add_header(crow::response& rjson)
		{
			rjson.add_header("Access-Control-Allow-Headers", "Content-Type");
			rjson.add_header("Access-Control-Allow-Methods", "OPTIONS, GET, POST, PUT, PATCH, DELETE");
			rjson.add_header("Accept-Encoding", "gzip");
			rjson.add_header("Access-Control-Allow-Origin", "*");
			rjson.code = OK;
		}
	};
	class url
	{
	public:
		inline url()
		{

		}
		inline ~url()
		{

		}
		static bool getMaxNumberParam(const int& size, const crow::query_string& req, crow::response& res)
		{
			res.code = 200;
			int tmp = static_cast<int>(req.key_value_pairs_.size());
			if (tmp > size)
			{
				crow::json::wvalue kq;	
				kq["code"] = BAD_REQUEST;
				kq["message"] = "invalid numbers of request params";
				res = crow::response(kq);
				validate::header::add_header(res); 
				return false;
			}
			return true;
		}
		static bool getNumberParam(const int& size, const crow::query_string& req, crow::response& res)
		{
			res.code = 200;
			int tmp = static_cast<int>(req.key_value_pairs_.size());
			if (tmp != size)
			{
				crow::json::wvalue kq; 
				kq["code"] = 151;
				kq["message"] = lstError[155];
				res = crow::response(kq);
				validate::header::add_header(res);
				return false;
			}
			return true; 
		}

		static std::string getValue(const int& type, crow::response& res, const crow::query_string& req, const std::string& key)
		{
			auto tmp = req.get(key);

			if (tmp == NULL)
			{
				crow::json::wvalue kq; 
                kq["code"] = 151;
				kq["message"] = lstError[151]; 
                kq["key"] = key;
                res = crow::response(kq); 
				validate::header::add_header(res);
				res.code = NOT_FOUND;
				return "";
			}
			if (std::string(tmp) == "")
			{
				crow::json::wvalue kq;
				kq["code"] = 152;
				kq["message"] = lstError[152]; 
				kq["key"] = key;
				res = crow::response(kq);
				validate::header::add_header(res);
				res.code = NOT_FOUND;
				return "";
			}
			try
			{
				switch (type)
				{
					case 0 :
					{
						bool b = (BOOL)std::stoi(tmp);
						break;
					}
					case 1 :
					{
						double d = std::stod(tmp);
						break;
					}
					case 2 :
					{
						int i = std::stoi(tmp);
						break;
					}
					case 3:
					{
                        std::string s = std::string(tmp); 
						break; 
					}
					case 4:
					{
						std::vector<int> i_arr = StringProcess::parseStrToInt(tmp);
						break;
					}
					case 5:
					{
						std::vector<double> d_arr = StringProcess::parseStrToDouble(tmp,',');
						break;
					}
					case 6:
					{
						std::vector<std::string> s_arr = StringProcess::parseURLToArray(tmp, ',');
						break;
					}
				    default:
					    break;
				}
				return std::string(tmp);
			}
			catch (std::exception& e)
			{
                crow::json::wvalue kq;
                kq["code"] = 201;
				kq["message"] = lstError[201]; 
				kq["detail"] = e.what();
                kq["key"] = key;
                kq["value"] = tmp; 
                res = crow::response(kq);
				validate::header::add_header(res);
				res.code = BAD_REQUEST;
				return "";
			}
		}
	};
	//------------------------------------------------
	class body
	{
	public:
		inline body() {}
		inline ~body() {}
		static bool getMaxNumberParam(const int& size, const crow::json::rvalue& req, crow::response& res)
		{
			res.code = 200;
			int tmp = static_cast<int>(req.size());
			if (tmp > size)
			{
				crow::json::wvalue kq;
				kq["code"] = BAD_REQUEST;
				kq["message"] = "invalid numbers of request params";
				res = crow::response(kq);
				validate::header::add_header(res);
				return false;
			}
			return true;
		}
		static bool getNumberParam(const int& size, const crow::json::rvalue& req, crow::response& res)
		{
			res.code = 200;
			int tmp = static_cast<int>(req.size());
			if (tmp != size)
			{
				crow::json::wvalue kq;
				kq["code"] = 151;
				kq["message"] = lstError[155];
				res = crow::response(kq);
				validate::header::add_header(res);
				return false;
			}
			return true;
		}
		static std::string getValue(const int& type, crow::response& res, const crow::json::rvalue& req, const std::string& key1 , std::string key2 = "")
		{
			try
			{
				if (!req.has(key1))
				{
					crow::json::wvalue kq;
					kq["code"] = 151;
					kq["message"] = lstError[151]; 
					kq["key"] = key1;
					res = crow::response(kq);
					validate::header::add_header(res);
					res.code = NOT_FOUND;
					return "";
				}

				auto tmp = req[key1];

				if (key1 != "" && key2 != "")
				{
					if (!tmp.has(key2))
					{
						crow::json::wvalue kq;
						kq["code"] = 151;
						kq["message"] = lstError[151];
						kq["key"] = key2;
						res = crow::response(kq);
						validate::header::add_header(res);
						res.code = NOT_FOUND;
						return "";
					}
					tmp = req[key1][key2];
				}

				switch (type)
				{
				case 0:
				{
					bool b = tmp.b();
					break;
				}
				case 1:
				{
					double d = tmp.d();
					break;
				}
				case 2:
				{
					int i = static_cast<int>(tmp.i());
					break;
				}
				case 3:
				{
					std::string s = tmp.s();
					if (s == "")
					{
						crow::json::wvalue kq;
						kq["code"] = 152;
						kq["message"] = lstError[152]; 
						kq["key"] = key1;
						res = crow::response(kq);
						validate::header::add_header(res);
						res.code = BAD_REQUEST;
						return "";
					}
					break;
				}
				case 4: 
				{
					return ""; 
					break; 
				}
				default:
					break;
				}
				return std::string(tmp);
			}
			catch (std::exception& e)
			{
				crow::json::wvalue kq;
				kq["code"] = 201;
				kq["message"] = lstError[201];
				kq["detail"] = e.what(); 
				kq["key"] = key1;
				if ( key2 != "")
					kq["key2"] = key2; 
				//kq["value"] = tmp;
				res = crow::response(kq);
				validate::header::add_header(res);
				res.code = BAD_REQUEST;
				return "";
			}
		}

		static std::string checkMultiPart(const crow::request& req, crow::response& res)
		{
			res.code = 200; 
			try
			{
				crow::multipart::message msg(req);

				return std::string("");
			}
			catch (std::exception e)
			{
				crow::json::wvalue kq;
				kq["code"] = 153;
				kq["message"] = lstError[153] ;
				res = crow::response(kq);
				validate::header::add_header(res);
				res.code = BAD_REQUEST;
				return "";
			}
		}

	private:

	};

}