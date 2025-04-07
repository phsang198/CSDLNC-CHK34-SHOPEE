#pragma once

#pragma region Request
#define RQ_RESOURCE												"func"
//---------------------------------------------------------------------------------
#pragma region clear cache

#define RQ_CLEAR_CACHE_URL										"/token/v1/clear"

#define RQ_CLEAR_CACHE_PARAM_SERVICE							"service"
#define RQ_CLEAR_CACHE_PARAM_PRODUCTID							"productid"
#define RQ_TOKEN_PARAM_HOLD_LOGIN								"hold_login"
#define RQ_TOKEN_PARAM_OLD_ACCESSTOKEN							"token"

#pragma endregion
//---------------------------------------------------------------------------------
#pragma region login
#define RQ_LOGIN_URL											"/token/v1/login"
#define RQ_RELOGIN_URL											"/token/v1/relogin"
#define RQ_LOGOUT_URL											"/token/v1/logout"
#define RQ_REGIGTER_URL											"/token/v1/register"
#define RQ_VALIDATE_URL											"/token/v1/validate"

#define RQ_TOKEN_PARAM_PRODUCTID								"productid"
#define RQ_TOKEN_PARAM_HOLD_LOGIN								"hold_login"
#define RQ_TOKEN_PARAM_OLD_ACCESSTOKEN							"token"
#pragma endregion
//---------------------------------------------------------------------------------
#pragma region user
#define RQ_USER_URL												"/admin/v1/user"
#define RQ_USER_LINK_URL										"/admin/v1/user/link"
#define RQ_USER_PARAM_ID										"id"
#define RQ_USER_PARAM_USER_NAME									"user_name"
#define RQ_USER_PARAM_PASSWORD									"password"
#define RQ_USER_PARAM_NEW_PASSWORD								"new_password"
#define RQ_USER_PARAM_NAME										"name"
#define RQ_USER_PARAM_AVATAR									"avatar"
#define RQ_USER_PARAM_EMAIL										"email"
#define RQ_USER_PARAM_ROLE										"role"
#define RQ_USER_PARAM_BUILDING									"building"
#define RQ_USER_PARAM_PHONE_NUMBER								"phone_number"
#define RQ_USER_PARAM_ADDRESS									"address"
#define RQ_USER_PARAM_SERVICE									"service"
#define RQ_USER_PARAM_PRODUCTID									"productid"
#define RQ_USER_PARAM_TYPE										"type"

#pragma endregion
//---------------------------------------------------------------------------------
#pragma region authorization
#define RQ_AUTHORIZATION_URL									"/admin/v1/authorization"
#define RQ_AUTHORIZATION_PERMISSION_URL							"/admin/v1/authorization/permission"
#define RQ_AUTHORIZATION_PARAM_USER_NAME						"user_name"
#define RQ_AUTHORIZATION_PARAM_ROLE								"role"
#define RQ_AUTHORIZATION_PARAM_USERID							"userid"
#define RQ_AUTHORIZATION_PARAM_ROLEID							"roleid"
#define RQ_AUTHORIZATION_PARAM_PRODUCTID						"productid"

#define RQ_AUTHORIZATION_PARAM_BUILDINGID						"buildingid"
#define RQ_TMP_AUTHORIZATION_PARAM_ID							"id"
#define RQ_TMP_AUTHORIZATION_PARAM_STATUS						"status"


#define RQ_AUTHORIZATION_PARAM_TYPE								"type"

#define RQ_AUTHORIZATION_PARAM_SERVICE							"service"
#define RQ_AUTHORIZATION_PARAM_INFO								"info"
#pragma endregion
//---------------------------------------------------------------------------------
#pragma region secret_key
#define RQ_SECRET_KEY_URL										"/token/v1/secretkey"
#define RQ_SET_SECRET_KEY_URL									"/token/v1/secretkey"
#define RQ_GET_SECRET_KEY_URL									"/token/v1/getSecretkey"

#define RQ_SECRET_KEY_PARAM_SERVICE								"service"
#pragma endregion

#pragma region product	
#define RQ_PRODUCT_URL											"/token/v1/product"

#define RQ_PRODUCT_PARAM_ID										"id"
#define RQ_PRODUCT_PARAM_SERVICE								"service"
#define RQ_PRODUCT_PARAM_PRODUCTID								"productid"
#define RQ_PRODUCT_PARAM_CONFIG									"config"
#pragma endregion
//---------------------------------------------------------------------------------
#pragma region role	
#define RQ_ROLE_URL												"/admin/v1/role"

#define RQ_ROLE_PARAM_ID										"id"
#define RQ_ROLE_PARAM_NAME										"name"
#define RQ_ROLE_PARAM_RANK										"rank"
#pragma endregion
//---------------------------------------------------------------------------------
#pragma region rbac	
#define RQ_RBAC_URL												"/admin/v1/rbac"
#define RQ_RBAC_PARAM_ID										"id"
#define RQ_RBAC_PARAM_ROLEID									"roleid"
#define RQ_RBAC_PARAM_APIID										"apiid"
#pragma endregion
//---------------------------------------------------------------------------------
#pragma region network	
#define RQ_SCHEDULE_URL											"/network/v1/connection"
#pragma endregion
//---------------------------------------------------------------------------------
#pragma region api	
#define RQ_API_URL												"/admin/v1/api"

#define RQ_API_PARAM_ID											"id"
#define RQ_API_PARAM_ROOT										"root"
#define RQ_API_PARAM_VERSION									"version"
#define RQ_API_PARAM_RESOURCE									"resource"
#define RQ_API_PARAM_SUB_RESOURCE								"sub_resource"
#define RQ_API_PARAM_METHOD										"method"
#define RQ_API_PARAM_ATTRIBUTE									"attribute"
#pragma endregion
//---------------------------------------------------------------------------------
#pragma region resource	
#define RQ_RESOURCE_URL											"/admin/v1/resource"

#define RQ_RESOURCE_PARAM_RESOURCENAME							"rscname"
#define RQ_RESOURCE_PARAM_RESOURCEID							"rscid"
#define RQ_RESOURCE_PARAM_AUID									"auid"
#define RQ_RESOURCE_PARAM_BELONGNAME							"belongname"
#define RQ_RESOURCE_PARAM_BELONGID								"belongid"
#pragma endregion
//---------------------------------------------------------------------------------
#pragma region schedule 
#define RQ_SCHEDULE_EXCUTE_URL									"/schedule/v1/excute"
#define RQ_SCHEDULE_STOP_URL									"/schedule/v1/stop"
#define RQ_SCHEDULE_START_URL									"/schedule/v1/restart"
#define RQ_SCHEDULE_TRY_URL										"/schedule/v1/try"
#define RQ_SCHEDULE_REGISTER_URL								"/schedule/v1/registration"
#define RQ_SCHEDULE_PARAM_SCHEDULENAME							"scdname"
#define RQ_SCHEDULE_PARAM_SCHEDULEID							"scdid"
#define RQ_SCHEDULE_PARAM_ID									"id"
#define RQ_SCHEDULE_PARAM_TYPE									"type"
#define RQ_SCHEDULE_PARAM_WORKFLOWID							"wid"
#define RQ_SCHEDULE_PARAM_INSTANCES								"instances"
#define RQ_SCHEDULE_PARAM_INSTANCEID							"iid"
#define RQ_SCHEDULE_PARAM_INPUT									"input"
#define RQ_SCHEDULE_PARAM_STARTTIME								"starttime"
#define RQ_SCHEDULE_PARAM_STR_STARTTIME							"sstarttime"
#define RQ_SCHEDULE_PARAM_STATUS								"status"
#define RQ_SCHEDULE_PARAM_INSTANCE_PTIME						"itimeout"
#define RQ_SCHEDULE_PARAM_WORKFLOW_PTIME						"wtimeout"
#pragma endregion
//---------------------------------------------------------------------------------
#pragma region webhook 
#define RQ_WEBHOOK_URL											"/schedule/v1/webhook"
#define RQ_TEST_URL												"/schedule/v1/test"
#pragma endregion
//---------------------------------------------------------------------------------
#pragma endregion
//==================================================================================================================
#pragma region Response
//---------------------------------------------------------------------------------
#pragma region format

#define RP_FORMAT_CODE											"code"
#define RP_FORMAT_MSG											"message"
#define RP_FORMAT_DATA											"data"
#pragma endregion 
//---------------------------------------------------------------------------------
#pragma region user
#define RP_USERS												"users"
#define RP_USER_PARAM_ID										"id"
#define RP_USER_PARAM_USER_NAME									"user_name"
#define RP_USER_PARAM_PASSWORD									"password"
#define RP_USER_PARAM_NAME										"name"
#define RP_USER_PARAM_AVATAR									"avatar"
#define RP_USER_PARAM_EMAIL										"email"
#define RP_USER_PARAM_ROLE										"role"
#define RP_USER_PARAM_BUILDING									"building"
#define RP_USER_PARAM_PHONE_NUMBER								"phone_number"
#define RP_USER_PARAM_ADDRESS									"address"
#define RP_USER_PARAM_SERVICE									"service"
#pragma endregion
//---------------------------------------------------------------------------------
#pragma region role

#define RP_AUTHO												"authorizations"
#define RP_TMP_AUTHO											"tmp_authorizations"
#define RP_ROLES												"roles"
#define RP_ROLES_PARAM_USER_NAME								"user_name"
#define RP_ROLES_PARAM_NAME										"name"
#define RP_ROLES_PARAM_ROLE										"role"
#define RP_ROLES_PARAM_INFO										"info"
#define RP_ROLES_PARAM_SERVICE									"service"

#define RP_AUTHO_PARAM_ID										"id"
#define RP_AUTHO_PARAM_USERID									"userid"
#define RP_AUTHO_PARAM_PRODUCTID								"productid"
#define RP_AUTHO_PARAM_ROLEID									"roleid"
#define RP_AUTHO_PARAM_ROLENAME									"rolename"
#define RP_AUTHO_PARAM_BUILDINGID								"buildingid"
#define RP_AUTHO_PARAM_STATUS									"status"
#pragma endregion
//---------------------------------------------------------------------------------
#pragma region product	
#define RP_PRODUCT												"product"
#define RP_PRODUCT_PARAM_SECRETKEY								"secretkey"
#define RP_PRODUCT_PARAM_SERVICE								"service"
#define RP_PRODUCT_PARAM_CONFIG									"config"
#define RP_PRODUCT_PARAM_ID										"id"
#define RP_PRODUCT_PARAM_TYPE									"type"
#pragma endregion
//---------------------------------------------------------------------------------
#pragma region role	
#define RP_ROLE													"roles"
#define RP_ROLE_PARAM_NAME										"name"
#define RP_ROLE_PARAM_ID										"id"
#define RP_ROLE_PARAM_RANK										"rank"
#pragma endregion
//---------------------------------------------------------------------------------
#pragma region rbac	
#define RP_RBAC													"rbacs"
#define RP_RBAC_PARAM_ID										"id"
#define RP_RBAC_PARAM_ROLEID									"roleid"
#define RP_RBAC_PARAM_APIID										"apiid"
#pragma endregion
//---------------------------------------------------------------------------------
#pragma region workflow	
#define RP_WORKFLOW												"workflows"
#define RP_WORKFLOW_PARAM_ID									"id"
#define RP_WORKFLOW_PARAM_SCRIPT								"script"
#define RP_WORKFLOW_PARAM_NAME									"name"
#define RP_WORKFLOW_PARAM_STATUS								"status"
#define RP_WORKFLOW_PARAM_DESC									"description"
#define RP_WORKFLOW_PARAM_CREATEDTIME							"createdtime"
#pragma endregion
//---------------------------------------------------------------------------------
#pragma region instance	
#define RP_INSTANCE												"instances"
#define RP_INSTANCE_PARAM_ID									"id"
#define RP_INSTANCE_PARAM_WFID									"workflowid"
#define RP_INSTANCE_PARAM_WFNAME								"workflowname"
#define RP_INSTANCE_PARAM_CREATEDTIME							"createdtime"
#define RP_INSTANCE_PARAM_STATUS								"status"
#pragma endregion
//---------------------------------------------------------------------------------
#pragma region model	
#define RP_MODEL												"models"
#define RP_REGIS												"registrations"
#define RP_MODEL_PARAM_ID										"id"
#define RP_MODEL_PARAM_REGISTERID								"registerid"
#define RP_MODEL_PARAM_SCRIPT									"script"
#define RP_MODEL_PARAM_AUID										"auid"
#define RP_MODEL_PARAM_MODELID									"modelid"
#define RP_MODEL_PARAM_STATUS									"status"
#pragma endregion
//---------------------------------------------------------------------------------
#pragma region api	
#define RP_API													"apis"
#define RP_API_PARAM_ID											"id"
#define RP_API_PARAM_ROOT										"root"
#define RP_API_PARAM_VERSION									"version"
#define RP_API_PARAM_RESOURCE									"resource"
#define RP_API_PARAM_SUB_RESOURCE								"sub_resource"
#define RP_API_PARAM_METHOD										"method"
#define RP_API_PARAM_ATTRIBUTE									"attribute"
#pragma endregion
//---------------------------------------------------------------------------------
#pragma region schedule 
#define RP_SCHEDULE												"schedules"
#define RP_SCHEDULE_PARAM_SCHEDULENAME							"scdname"
#define RP_SCHEDULE_PARAM_ID									"id"
#define RP_SCHEDULE_PARAM_WORKFLOWID							"wid"
#define RP_SCHEDULE_PARAM_INSTANCES								"instances"
#define RP_SCHEDULE_PARAM_INSTANCEID							"iid"
#define RP_SCHEDULE_PARAM_INPUT									"input"
#define RP_SCHEDULE_PARAM_STARTTIME								"starttime"
#define RP_SCHEDULE_PARAM_STATUS								"status"
#define RP_SCHEDULE_PARAM_INSTANCE_PTIME						"itimeout"
#define RP_SCHEDULE_PARAM_WORKFLOW_PTIME						"wtimeout"
#pragma endregion
//---------------------------------------------------------------------------------
#pragma endregion
//==================================================================================================================
#pragma region VBD API
#define AUTO_SUGGEST_SEARCH		"/indoors/v1/AutoSuggestSearch"
#define GEO_CODING				"/indoors/v1/GeoCoding"
#define SEARCH_ALL				"/indoors/v1/SearchAll"
#define FIND_SHORTEST_PATH		"/indoors/v1/FindShortestPath"
#pragma endregion