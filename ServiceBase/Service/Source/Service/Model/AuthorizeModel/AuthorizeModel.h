#pragma once

class authorize;
class tmp_authorize; 
class AuthorizeModel
{
public:
	AuthorizeModel();
	~AuthorizeModel();
public:
	//-------------------------------------------------------------------------------------------------------------------------
	int getAllAutho(authorize*& autho);
	int processGetAutho(std::string user_name, std::string password, std::string productid, std::vector<authorize>& listAutho);
	int updateAutho(authorize _autho, std::string new_autho);
	int deleteAutho(std::string id);
	int createAutho(authorize& _autho);
	int createAuthoFromTmpAutho(std::string& tmp_autho_id);
	int getAutho(std::string type, std::string productid, std::string auid, std::vector<authorize>& listAutho);

	int createTmpAutho(tmp_authorize& _autho);
	int updateTmpAutho(std::string id, std::string status);
	int deleteTmpAutho(std::string id, std::string userid);
	int getTmpAutho(std::string type, std::string auid, std::vector<tmp_authorize>& tmp_autho, std::string id = "");
private:

};
