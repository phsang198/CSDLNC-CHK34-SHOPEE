#include "pch.h"
#include "Rbac.h"
#include "CApi/CApi.h"

std::map<std::string, std::map<std::string,BOOL>> rbacManager::lst_rbac;

rbac::rbac()
{

}
rbac::rbac(std::string _roleid, std::string _apiid, std::string _id )
{
	roleid = _roleid;
	apiid = _apiid;
	id = _id;
}

void rbacManager::vector2map(std::vector<rbac> vetor_rbac)
{
	for (auto x : vetor_rbac)
	{
		CApi _api = apiManager::lst_api[x.apiid]; 
		std::string url = _api.method+"/"+_api.root + "/" + _api.version + "/" + _api.resource +
						 ((_api.sub_resource != "") ? ("/" + _api.sub_resource) : "");
		lst_rbac[x.roleid][url] = TRUE;
	}
}
