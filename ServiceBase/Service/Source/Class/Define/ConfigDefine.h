#pragma once

#pragma region s3w
#define FOLDER_EDITOR "editor"
#define FOLDER_PRODUCTS "products"
#define FOLDER_FLOORS "floors"
#define FILE_BUILDING "building"
#define FILE_EDITOR_OSM "editor/building.editor.osm"
#define FILE_EDITOR_JSON "editor/building.editor.json"
#define FILE_JSON "json" 
#define FILE_GEOJSON "geojson" 
#define FILE_PBF "pbf"
#define FILE_WEB "web"
#define FILE_DESKTOP "desktop"
#define FILE_OSM "osm"
#define FILE_XML "xml"
#define FILE_NETWORK "network"
#pragma endregion

#pragma region BuildEvent

#define FILE_DATA_TMP "data/tmp/"
#define FILE_DATA_PRODUCTS "data/products/"

#define FOLDER_DATA_PRODUCTS "data/products"
#define FOLDER_DATA "data/"
#define FOLDER_DATA_TMP "data/tmp"
#define FOLDER_SHARE "share/"
#pragma endregion

#pragma region ScheduleService
#define SERVICE "service_config"
#define SERVICE_PORT "port"
#define SERVICE_MAXTHREADS "maxThreads"
#define SERVICE_MAXQUEUED "maxQueued"
#define SERVICE_TIMEOUT "timeOut"
#pragma endregion

#pragma region VBDAPI
#define VBDAPI "VBD_API"
#define URLVBD "URLVBD"
#define VBDKEY "VBDKEY"
#pragma endregion

#pragma region VBDAPI
#define ROUTEAPI "Route_API"
#define ROUTE_SERVICE "Route_service"
#define TABLE_SERVICE "Table_service"
#pragma endregion

#pragma region Database

#define POSTGRES_DB "postgres_db_config"
#define PDB_DATABASE_NAME "database_name"
#define PDB_HOST "host"
#define PDB_PORT "port"
#define PDB_USER "user"
#define PDB_PASSWORD "password"
#pragma endregion

#pragma region Provider
#define BUILDING_DATA_PROVIDER "provider_config"
#define BDP_NAME "name"
#define BDP_TYPE "type"
#define BDP_SECRET_CONFIG "secret_config"
#define BDP_PARAM "parameters"
#define BDP_SC_ACCESSKEYID "accessKeyID"
#define BDP_SC_SCACCESSKEY "secretAccessKey"

#define BDP_PARAM_HOST "host"
#define BDP_PARAM_BUCKETNAME "bucketName"
#define BDP_PARAM_REGION "region"

#define BDP_PARAM_FOLDER "folder"
#pragma endregion

#pragma region POI
#define PARTICULAR "particular"
#define POINAME "poi_name"
#define LISTFIELD "list_field"
#define FIELDNAME "field_name"
#define FIELDTYPE "field_type"
#pragma endregion

#pragma region ERROR
#define DESCRIBE "describe"
#define CODE "code"
#define NAME "name"
#pragma endregion

#define FILE_CONFIG_PATH "config/config.json"
#define FILE_SCHEMA_CONFIG_PATH "config/schema.sql"
#define RUN_PROJECT "Service.exe"