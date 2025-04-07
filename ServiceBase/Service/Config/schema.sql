--
-- PostgreSQL database dump
--

-- Dumped from database version 14.4
-- Dumped by pg_dump version 14.4

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

--
-- Name: pg_trgm; Type: EXTENSION; Schema: -; Owner: -
--

CREATE EXTENSION IF NOT EXISTS pg_trgm WITH SCHEMA public;


--
-- Name: EXTENSION pg_trgm; Type: COMMENT; Schema: -; Owner: 
--

COMMENT ON EXTENSION pg_trgm IS 'text similarity measurement and index searching based on trigrams';


--
-- Name: postgis; Type: EXTENSION; Schema: -; Owner: -
--

CREATE EXTENSION IF NOT EXISTS postgis WITH SCHEMA public;


--
-- Name: EXTENSION postgis; Type: COMMENT; Schema: -; Owner: 
--

COMMENT ON EXTENSION postgis IS 'PostGIS geometry and geography spatial types and functions';


--
-- Name: unaccent; Type: EXTENSION; Schema: -; Owner: -
--

CREATE EXTENSION IF NOT EXISTS unaccent WITH SCHEMA public;


--
-- Name: EXTENSION unaccent; Type: COMMENT; Schema: -; Owner: 
--

COMMENT ON EXTENSION unaccent IS 'text search dictionary that removes accents';


--
-- Name: uuid-ossp; Type: EXTENSION; Schema: -; Owner: -
--

CREATE EXTENSION IF NOT EXISTS "uuid-ossp" WITH SCHEMA public;


--
-- Name: EXTENSION "uuid-ossp"; Type: COMMENT; Schema: -; Owner: 
--

COMMENT ON EXTENSION "uuid-ossp" IS 'generate universally unique identifiers (UUIDs)';


--
-- Name: func_train_beacons(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION public.func_train_beacons() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
BEGIN
  NEW.modified = NOW();
  RETURN NEW;
END;
$$;


ALTER FUNCTION public.func_train_beacons() OWNER TO postgres;

--
-- Name: init_created_time(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION public.init_created_time() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
BEGIN
    NEW.created = now();
    RETURN NEW;
END;
$$;


ALTER FUNCTION public.init_created_time() OWNER TO postgres;

--
-- Name: init_modified_time(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION public.init_modified_time() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
BEGIN
    NEW.modified = now();
    RETURN NEW;
END;
$$;


ALTER FUNCTION public.init_modified_time() OWNER TO postgres;

--
-- Name: inseart_table(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION public.inseart_table() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
BEGIN
    NEW.created = now();
    RETURN NEW;
END;
$$;


ALTER FUNCTION public.inseart_table() OWNER TO postgres;

--
-- Name: insert_table(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION public.insert_table() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
BEGIN
    NEW.created = now();
    RETURN NEW;
END;
$$;


ALTER FUNCTION public.insert_table() OWNER TO postgres;

--
-- Name: trigger_set_timestamp(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION public.trigger_set_timestamp() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
BEGIN
  NEW.modified = NOW();
  RETURN NEW;
END;
$$;


ALTER FUNCTION public.trigger_set_timestamp() OWNER TO postgres;

--
-- Name: ts_trigger_func(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION public.ts_trigger_func() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
BEGIN NEW.ts_content =
	setweight(to_tsvector(coalesce(vn_unaccent(NEW.name))), 'A') ||
	setweight(to_tsvector(coalesce(vn_unaccent(NEW.address))), 'B');
RETURN NEW;
END $$;


ALTER FUNCTION public.ts_trigger_func() OWNER TO postgres;

--
-- Name: update_created_time(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION public.update_created_time() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
BEGIN
    NEW.created = now();
    RETURN NEW;
END;
$$;


ALTER FUNCTION public.update_created_time() OWNER TO postgres;

--
-- Name: update_modified_a_time(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION public.update_modified_a_time() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
BEGIN
    NEW.modified = now();
    RETURN NEW;
END;
$$;


ALTER FUNCTION public.update_modified_a_time() OWNER TO postgres;

--
-- Name: update_modified_time(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION public.update_modified_time() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
BEGIN
    NEW.modified = now();
    RETURN NEW;
END;
$$;


ALTER FUNCTION public.update_modified_time() OWNER TO postgres;

--
-- Name: update_table(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION public.update_table() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
BEGIN
    NEW.modified = now();
    RETURN NEW;
END;
$$;


ALTER FUNCTION public.update_table() OWNER TO postgres;

--
-- Name: update_timestam(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION public.update_timestam() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
BEGIN
    NEW.modified = CURRENT_TIMESTAMP;
    RETURN NEW;
END;
$$;


ALTER FUNCTION public.update_timestam() OWNER TO postgres;

--
-- Name: update_timestamp(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION public.update_timestamp() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
BEGIN
    NEW.modified = CURRENT_TIMESTAMP;
    RETURN NEW;
END;
$$;


ALTER FUNCTION public.update_timestamp() OWNER TO postgres;

--
-- Name: vn_unaccent(text); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION public.vn_unaccent(text) RETURNS text
    LANGUAGE sql IMMUTABLE
    AS $_$
SELECT lower(translate($1,
'¹²³ÀÁẢẠÂẤẦẨẬẪÃÄÅÆàáảạâấầẩẫậãäåæĀāĂẮẰẲẴẶăắằẳẵặĄąÇçĆćĈĉĊċČčĎďĐđÈÉẸÊẾỀỄỆËèéẹêềếễệëĒēĔĕĖėĘęĚěĜĝĞğĠġĢģĤĥĦħĨÌÍỈỊÎÏìíỉịîïĩĪīĬĭĮįİıĲĳĴĵĶķĸĹĺĻļĽľĿŀŁłÑñŃńŅņŇňŉŊŋÒÓỎỌÔỐỒỔỖỘỐỒỔỖỘƠỚỜỞỠỢÕÖòóỏọôốồổỗộơớờỡợởõöŌōŎŏŐőŒœØøŔŕŖŗŘřßŚśŜŝŞşŠšŢţŤťŦŧÙÚỦỤƯỪỨỬỮỰÛÜùúủụûưứừửữựüŨũŪūŬŭŮůŰűŲųŴŵÝýÿŶŷŸŹźŻżŽžёЁ',
'123AAAAAAAAAAAAAAaaaaaaaaaaaaaaAaAAAAAAaaaaaaAaCcCcCcCcCcDdDdEEEEEEEEEeeeeeeeeeEeEeEeEeEeGgGgGgGgHhHhIIIIIIIiiiiiiiIiIiIiIiIiJjKkkLlLlLlLlLlNnNnNnNnnNnOOOOOOOOOOOOOOOOOOOOOOOooooooooooooooooooOoOoOoEeOoRrRrRrSSsSsSsSsTtTtTtUUUUUUUUUUUUuuuuuuuuuuuuUuUuUuUuUuUuWwYyyYyYZzZzZzеЕ'));
$_$;


ALTER FUNCTION public.vn_unaccent(text) OWNER TO postgres;

--
-- Name: z(numeric); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION public.z(scaledenominator numeric) RETURNS integer
    LANGUAGE plpgsql IMMUTABLE
    AS $$
begin
    -- Don't bother if the scale is larger than ~zoom level 0
    if scaledenominator > 600000000 then
        return null;
    end if;
    return round(log(2,559082264.028/scaledenominator));
end;
$$;


ALTER FUNCTION public.z(scaledenominator numeric) OWNER TO postgres;

SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- Name: area; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.area (
    id uuid DEFAULT public.uuid_generate_v4() NOT NULL,
    name character varying NOT NULL,
    floorid uuid NOT NULL,
    boundingbox jsonb
);


ALTER TABLE public.area OWNER TO postgres;

--
-- Name: authorization; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."authorization" (
    id uuid DEFAULT public.uuid_generate_v4() NOT NULL,
    userid uuid NOT NULL,
    roleid uuid NOT NULL,
    productid uuid NOT NULL
);


ALTER TABLE public."authorization" OWNER TO postgres;

--
-- Name: beacon; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.beacon (
    mac character varying(24) NOT NULL,
    name character varying(64) NOT NULL,
    major integer,
    minor integer,
    firmware_a_version character varying(128),
    firmware_b_version character varying(128),
    ap_mac character varying(17),
    type character varying(24) DEFAULT 'proximity'::character varying,
    map_type smallint DEFAULT 0,
    location point,
    rssi integer,
    battery_level real,
    txpower integer,
    observed timestamp(6) without time zone,
    observer integer,
    created timestamp(6) without time zone NOT NULL,
    modified timestamp(6) without time zone NOT NULL,
    height double precision,
    direction double precision,
    expcof double precision,
    rssicof double precision,
    id uuid DEFAULT public.uuid_generate_v4() NOT NULL,
    areaid uuid NOT NULL
);


ALTER TABLE public.beacon OWNER TO postgres;

--
-- Name: COLUMN beacon.mac; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN public.beacon.mac IS 'A Beacon’s MAC';


--
-- Name: COLUMN beacon.name; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN public.beacon.name IS 'The Beacon name';


--
-- Name: COLUMN beacon.major; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN public.beacon.major IS 'The Beacon’s major value';


--
-- Name: COLUMN beacon.minor; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN public.beacon.minor IS 'The Beacon’s minor value';


--
-- Name: COLUMN beacon.firmware_a_version; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN public.beacon.firmware_a_version IS 'The Beacon firmware version';


--
-- Name: COLUMN beacon.firmware_b_version; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN public.beacon.firmware_b_version IS 'The Beacon firmware version';


--
-- Name: COLUMN beacon.ap_mac; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN public.beacon.ap_mac IS 'If this beacon is attached to an Access Point (AP), the Beacon’s AP MAC address';


--
-- Name: COLUMN beacon.type; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN public.beacon.type IS 'Whether a Beacon is configured as a location or proximity Beacon';


--
-- Name: COLUMN beacon.map_type; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN public.beacon.map_type IS '0: map, 1: imagery';


--
-- Name: COLUMN beacon.location; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN public.beacon.location IS 'WGS84';


--
-- Name: COLUMN beacon.rssi; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN public.beacon.rssi IS 'The Beacon received signal strength indication';


--
-- Name: COLUMN beacon.battery_level; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN public.beacon.battery_level IS 'The Beacon battery level';


--
-- Name: COLUMN beacon.txpower; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN public.beacon.txpower IS 'Beacon transmission power';


--
-- Name: COLUMN beacon.observed; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN public.beacon.observed IS 'The last time the Beacon was observed';


--
-- Name: COLUMN beacon.observer; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN public.beacon.observer IS 'The ID of the Beacon’s observer for the most recent timestamp.';


--
-- Name: COLUMN beacon.created; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN public.beacon.created IS 'Timestamps for when a Beacon was deployed to a map and when a Beacon’s state was last changed.';


--
-- Name: COLUMN beacon.modified; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN public.beacon.modified IS 'Timestamps for when a Beacon was deployed to a map and when a Beacon’s state was last changed.';


--
-- Name: beaconmanager; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.beaconmanager (
    id uuid DEFAULT public.uuid_generate_v4() NOT NULL,
    beaconid uuid NOT NULL,
    auid uuid NOT NULL
);


ALTER TABLE public.beaconmanager OWNER TO postgres;

--
-- Name: building; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.building (
    name text,
    type text,
    height real,
    minheight real,
    levels real,
    minlevel real,
    color text,
    material text,
    shape text,
    roofshape text,
    roofheight real,
    rooflevels real,
    roofcolor text,
    roofmaterial text,
    roofdirection real,
    roofangle real,
    geom public.geometry(Geometry,4326),
    area real,
    footprint boolean DEFAULT false,
    buildingparts smallint DEFAULT 1,
    vcolor character varying(254),
    gid bigint NOT NULL,
    guid uuid DEFAULT public.uuid_generate_v4() NOT NULL,
    created timestamp without time zone,
    modified timestamp without time zone,
    existdata boolean,
    id uuid DEFAULT public.uuid_generate_v4() NOT NULL
);


ALTER TABLE public.building OWNER TO postgres;

--
-- Name: buildingmanager; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.buildingmanager (
    id uuid DEFAULT public.uuid_generate_v4() NOT NULL,
    buildingid uuid NOT NULL,
    auid uuid NOT NULL
);


ALTER TABLE public.buildingmanager OWNER TO postgres;

--
-- Name: buildings_vietnam_v2_gid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.buildings_vietnam_v2_gid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.buildings_vietnam_v2_gid_seq OWNER TO postgres;

--
-- Name: buildings_vietnam_v2_gid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.buildings_vietnam_v2_gid_seq OWNED BY public.building.gid;


--
-- Name: buildings_vietnam_v2_gid_seq1; Type: SEQUENCE; Schema: public; Owner: postgres
--

ALTER TABLE public.building ALTER COLUMN gid ADD GENERATED ALWAYS AS IDENTITY (
    SEQUENCE NAME public.buildings_vietnam_v2_gid_seq1
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1
);


--
-- Name: camerameta; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.camerameta (
    gps_position public.geometry,
    snap_position public.geometry,
    heading double precision DEFAULT 0.0,
    street_name character varying(256),
    datetime_taken double precision NOT NULL,
    trip_ref character varying(256) DEFAULT ''::character varying NOT NULL,
    visible boolean DEFAULT true NOT NULL,
    owner character varying(36) DEFAULT 'vbd_streetview'::character varying NOT NULL,
    trip_guid uuid,
    id uuid DEFAULT public.uuid_generate_v4()
);


ALTER TABLE public.camerameta OWNER TO postgres;

--
-- Name: category; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.category (
    id uuid DEFAULT public.uuid_generate_v4() NOT NULL,
    name character varying NOT NULL,
    field jsonb NOT NULL
);


ALTER TABLE public.category OWNER TO postgres;

--
-- Name: categorybuilding; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.categorybuilding (
    id uuid DEFAULT public.uuid_generate_v4() NOT NULL,
    buildingid uuid NOT NULL,
    categoryid uuid NOT NULL
);


ALTER TABLE public.categorybuilding OWNER TO postgres;

--
-- Name: constraint; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."constraint" (
    buildingid uuid NOT NULL,
    levelabove double precision NOT NULL,
    levelbelow double precision NOT NULL,
    objectname character varying NOT NULL,
    info jsonb,
    type character varying NOT NULL,
    hash character varying NOT NULL,
    id uuid DEFAULT public.uuid_generate_v4() NOT NULL
);


ALTER TABLE public."constraint" OWNER TO postgres;

--
-- Name: COLUMN "constraint".type; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN public."constraint".type IS 'type of object';


--
-- Name: constraintmanager; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.constraintmanager (
    id uuid DEFAULT public.uuid_generate_v4() NOT NULL,
    constraintid uuid NOT NULL,
    auid uuid NOT NULL
);


ALTER TABLE public.constraintmanager OWNER TO postgres;

--
-- Name: floor; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.floor (
    buildingid uuid DEFAULT 'b64b5478-1040-46ed-b421-e83596760d3c'::uuid NOT NULL,
    level double precision DEFAULT 0 NOT NULL,
    name character varying DEFAULT 'G'::character varying NOT NULL,
    id uuid DEFAULT public.uuid_generate_v4() NOT NULL
);


ALTER TABLE public.floor OWNER TO postgres;

--
-- Name: gis_osm_buildings_a_free_1; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.gis_osm_buildings_a_free_1 (
    gid integer NOT NULL,
    osm_id character varying(10),
    code smallint,
    fclass character varying(28),
    name character varying(100),
    type character varying(20),
    geom public.geometry(MultiPolygon,4326),
    test bytea
);


ALTER TABLE public.gis_osm_buildings_a_free_1 OWNER TO postgres;

--
-- Name: gis_osm_buildings_a_free_1_gid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.gis_osm_buildings_a_free_1_gid_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.gis_osm_buildings_a_free_1_gid_seq OWNER TO postgres;

--
-- Name: gis_osm_buildings_a_free_1_gid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.gis_osm_buildings_a_free_1_gid_seq OWNED BY public.gis_osm_buildings_a_free_1.gid;


--
-- Name: log_beacon; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.log_beacon (
    uuid character varying(64),
    mac character varying(24),
    name character varying(64),
    major integer,
    minor integer,
    rssi integer,
    txpower integer,
    connected timestamp without time zone,
    buildingid uuid
);


ALTER TABLE public.log_beacon OWNER TO postgres;

--
-- Name: log_token; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.log_token (
    id uuid DEFAULT public.uuid_generate_v4(),
    value character varying,
    created timestamp without time zone,
    ip character varying,
    productid uuid,
    expire bigint,
    "expireRefresh" bigint,
    "expireLogin" bigint
);


ALTER TABLE public.log_token OWNER TO postgres;

--
-- Name: mapstyle3d; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.mapstyle3d (
    id uuid,
    name character varying,
    info jsonb
);


ALTER TABLE public.mapstyle3d OWNER TO postgres;

--
-- Name: poi; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.poi (
    name character varying(255) NOT NULL,
    geom public.geometry NOT NULL,
    info jsonb NOT NULL,
    address character varying,
    categoryid uuid NOT NULL,
    id uuid DEFAULT public.uuid_generate_v4() NOT NULL,
    floorid uuid,
    ts_content tsvector
);


ALTER TABLE public.poi OWNER TO postgres;

--
-- Name: poimanager; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.poimanager (
    id uuid DEFAULT public.uuid_generate_v4() NOT NULL,
    poiid uuid NOT NULL,
    auid uuid NOT NULL
);


ALTER TABLE public.poimanager OWNER TO postgres;

--
-- Name: product; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.product (
    name character varying NOT NULL,
    type character varying,
    secretkey character varying NOT NULL,
    config jsonb NOT NULL,
    id uuid DEFAULT public.uuid_generate_v4() NOT NULL
);


ALTER TABLE public.product OWNER TO postgres;

--
-- Name: radiomap; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.radiomap (
    length double precision NOT NULL,
    width double precision NOT NULL,
    bnumber integer,
    length_scale double precision NOT NULL,
    output_scale double precision NOT NULL,
    alpha double precision NOT NULL,
    rssimatrix jsonb,
    boundingbox jsonb NOT NULL,
    trainbeacon jsonb NOT NULL,
    id uuid DEFAULT public.uuid_generate_v4() NOT NULL,
    areaid uuid NOT NULL
);


ALTER TABLE public.radiomap OWNER TO postgres;

--
-- Name: radiomapmanager; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.radiomapmanager (
    id uuid DEFAULT public.uuid_generate_v4() NOT NULL,
    radiomapid uuid NOT NULL,
    auid uuid NOT NULL
);


ALTER TABLE public.radiomapmanager OWNER TO postgres;

--
-- Name: restream; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.restream (
    host character varying NOT NULL,
    name character varying,
    id uuid DEFAULT public.uuid_generate_v4() NOT NULL,
    provider character varying
);


ALTER TABLE public.restream OWNER TO postgres;

--
-- Name: restreammanager; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.restreammanager (
    id uuid DEFAULT public.uuid_generate_v4() NOT NULL,
    restreamid uuid NOT NULL,
    auid uuid NOT NULL
);


ALTER TABLE public.restreammanager OWNER TO postgres;

--
-- Name: role; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.role (
    name character varying NOT NULL,
    id uuid DEFAULT public.uuid_generate_v4() NOT NULL,
    rank character varying
);


ALTER TABLE public.role OWNER TO postgres;

--
-- Name: stnmt_nhas_3d; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.stnmt_nhas_3d (
    gid integer NOT NULL,
    featureid double precision,
    guid character varying(36),
    parentguid character varying(1),
    manhandang character varying(17),
    phienban numeric,
    ngayphienb character varying(19),
    giatridoch numeric,
    nguyennhan character varying(2),
    madoituong character varying(4),
    loainha numeric,
    mucdokienc numeric,
    chieucao numeric,
    sotang numeric,
    ten character varying(70),
    shape_leng numeric,
    shape_area numeric,
    geom public.geometry
);


ALTER TABLE public.stnmt_nhas_3d OWNER TO postgres;

--
-- Name: stnmt_nhas_3d_gid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.stnmt_nhas_3d_gid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.stnmt_nhas_3d_gid_seq OWNER TO postgres;

--
-- Name: stnmt_nhas_3d_gid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.stnmt_nhas_3d_gid_seq OWNED BY public.stnmt_nhas_3d.gid;


--
-- Name: user; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."user" (
    user_name character varying NOT NULL,
    password character varying NOT NULL,
    name character varying,
    email character varying NOT NULL,
    phone_number character varying,
    avatar bytea,
    address character varying,
    id uuid DEFAULT public.uuid_generate_v4() NOT NULL
);


ALTER TABLE public."user" OWNER TO postgres;

--
-- Name: usermanager; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.usermanager (
    id uuid NOT NULL,
    auid uuid NOT NULL,
    userid uuid NOT NULL
);


ALTER TABLE public.usermanager OWNER TO postgres;

--
-- Name: gis_osm_buildings_a_free_1 gid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.gis_osm_buildings_a_free_1 ALTER COLUMN gid SET DEFAULT nextval('public.gis_osm_buildings_a_free_1_gid_seq'::regclass);


--
-- Name: stnmt_nhas_3d gid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.stnmt_nhas_3d ALTER COLUMN gid SET DEFAULT nextval('public.stnmt_nhas_3d_gid_seq'::regclass);


--
-- Name: area area_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.area
    ADD CONSTRAINT area_pkey PRIMARY KEY (id);


--
-- Name: authorization authorization_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."authorization"
    ADD CONSTRAINT authorization_pkey PRIMARY KEY (id);


--
-- Name: beaconmanager beaconmanager_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.beaconmanager
    ADD CONSTRAINT beaconmanager_pkey PRIMARY KEY (id);


--
-- Name: beacon beacons_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.beacon
    ADD CONSTRAINT beacons_pkey PRIMARY KEY (id);


--
-- Name: building building_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.building
    ADD CONSTRAINT building_pkey PRIMARY KEY (id);


--
-- Name: buildingmanager buildingmanager_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.buildingmanager
    ADD CONSTRAINT buildingmanager_pkey PRIMARY KEY (id);


--
-- Name: constraint c_constraint_hash; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."constraint"
    ADD CONSTRAINT c_constraint_hash UNIQUE (hash);


--
-- Name: category category_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.category
    ADD CONSTRAINT category_pkey PRIMARY KEY (id);


--
-- Name: categorybuilding categorybuilding_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.categorybuilding
    ADD CONSTRAINT categorybuilding_pkey PRIMARY KEY (id);


--
-- Name: constraintmanager constraintmanager_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.constraintmanager
    ADD CONSTRAINT constraintmanager_pkey PRIMARY KEY (id);


--
-- Name: constraint constraints_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."constraint"
    ADD CONSTRAINT constraints_pkey PRIMARY KEY (id);


--
-- Name: radiomap cs_unique_areaid; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.radiomap
    ADD CONSTRAINT cs_unique_areaid UNIQUE (areaid);


--
-- Name: floor floorname_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.floor
    ADD CONSTRAINT floorname_pkey PRIMARY KEY (id);


--
-- Name: gis_osm_buildings_a_free_1 gis_osm_buildings_a_free_1_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.gis_osm_buildings_a_free_1
    ADD CONSTRAINT gis_osm_buildings_a_free_1_pkey PRIMARY KEY (gid);


--
-- Name: poimanager poimanager_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.poimanager
    ADD CONSTRAINT poimanager_pkey PRIMARY KEY (id);


--
-- Name: poi pois_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.poi
    ADD CONSTRAINT pois_pkey PRIMARY KEY (id);


--
-- Name: product product_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.product
    ADD CONSTRAINT product_pkey PRIMARY KEY (id);


--
-- Name: radiomap radiomap_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.radiomap
    ADD CONSTRAINT radiomap_pkey PRIMARY KEY (id);


--
-- Name: radiomapmanager radiomapmanager_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.radiomapmanager
    ADD CONSTRAINT radiomapmanager_pkey PRIMARY KEY (id);


--
-- Name: restream restream_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.restream
    ADD CONSTRAINT restream_pkey PRIMARY KEY (id);


--
-- Name: restreammanager restreammanager_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.restreammanager
    ADD CONSTRAINT restreammanager_pkey PRIMARY KEY (id);


--
-- Name: role role_pkey1; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.role
    ADD CONSTRAINT role_pkey1 PRIMARY KEY (id);


--
-- Name: stnmt_nhas_3d stnmt_nhas_3d_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.stnmt_nhas_3d
    ADD CONSTRAINT stnmt_nhas_3d_pkey PRIMARY KEY (gid);


--
-- Name: floor unique_buildingid_level; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.floor
    ADD CONSTRAINT unique_buildingid_level UNIQUE (level, buildingid);


--
-- Name: categorybuilding unique_categorybuilding; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.categorybuilding
    ADD CONSTRAINT unique_categorybuilding UNIQUE (categoryid, buildingid);


--
-- Name: user unique_email; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."user"
    ADD CONSTRAINT unique_email UNIQUE (email);


--
-- Name: product unique_product_name; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.product
    ADD CONSTRAINT unique_product_name UNIQUE (name);


--
-- Name: role unique_role_name; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.role
    ADD CONSTRAINT unique_role_name UNIQUE (name);


--
-- Name: user unique_user_name; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."user"
    ADD CONSTRAINT unique_user_name UNIQUE (user_name);


--
-- Name: user user_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."user"
    ADD CONSTRAINT user_pkey PRIMARY KEY (id);


--
-- Name: usermanager usermanager_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.usermanager
    ADD CONSTRAINT usermanager_pkey PRIMARY KEY (id);


--
-- Name: buildings_vietnam_v2_geom_idx; Type: INDEX; Schema: public; Owner: postgres
--

CREATE INDEX buildings_vietnam_v2_geom_idx ON public.building USING gist (geom) WITH (fillfactor='100');


--
-- Name: camera_meta_full_gps_position_idx; Type: INDEX; Schema: public; Owner: postgres
--

CREATE INDEX camera_meta_full_gps_position_idx ON public.camerameta USING gist (gps_position);


--
-- Name: camera_meta_full_snap_position_idx; Type: INDEX; Schema: public; Owner: postgres
--

CREATE INDEX camera_meta_full_snap_position_idx ON public.camerameta USING gist (snap_position);


--
-- Name: gis_osm_buildings_a_free_1_geom_idx; Type: INDEX; Schema: public; Owner: postgres
--

CREATE INDEX gis_osm_buildings_a_free_1_geom_idx ON public.gis_osm_buildings_a_free_1 USING gist (geom);


--
-- Name: ipt_beacons_mac_idx; Type: INDEX; Schema: public; Owner: postgres
--

CREATE UNIQUE INDEX ipt_beacons_mac_idx ON public.beacon USING btree (mac);


--
-- Name: spatial_index_name; Type: INDEX; Schema: public; Owner: postgres
--

CREATE INDEX spatial_index_name ON public.poi USING gist (geom);


--
-- Name: stnmt_nhas_3d_geom_idx; Type: INDEX; Schema: public; Owner: postgres
--

CREATE INDEX stnmt_nhas_3d_geom_idx ON public.stnmt_nhas_3d USING gist (geom);


--
-- Name: ts_idx; Type: INDEX; Schema: public; Owner: postgres
--

CREATE INDEX ts_idx ON public.poi USING gin (ts_content);


--
-- Name: beacon created_insert_beacon_trig; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER created_insert_beacon_trig BEFORE INSERT ON public.beacon FOR EACH ROW EXECUTE FUNCTION public.inseart_table();


--
-- Name: log_token created_insert_token_log_trig; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER created_insert_token_log_trig BEFORE INSERT ON public.log_token FOR EACH ROW EXECUTE FUNCTION public.insert_table();


--
-- Name: building insert_modified_a_time_trig; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER insert_modified_a_time_trig BEFORE INSERT ON public.building FOR EACH ROW EXECUTE FUNCTION public.update_modified_a_time();


--
-- Name: beacon modified_insert_beacon_trig; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER modified_insert_beacon_trig BEFORE INSERT ON public.beacon FOR EACH ROW EXECUTE FUNCTION public.update_table();


--
-- Name: beacon modified_update_beacon_trig; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER modified_update_beacon_trig BEFORE UPDATE ON public.beacon FOR EACH ROW EXECUTE FUNCTION public.update_table();


--
-- Name: poi ts_trigger; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER ts_trigger BEFORE INSERT OR UPDATE OF name, address ON public.poi FOR EACH ROW EXECUTE FUNCTION public.ts_trigger_func();


--
-- Name: building update_created_time_trig; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER update_created_time_trig BEFORE INSERT ON public.building FOR EACH ROW EXECUTE FUNCTION public.update_created_time();


--
-- Name: building update_modified_a_time_trig; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER update_modified_a_time_trig BEFORE UPDATE ON public.building FOR EACH ROW EXECUTE FUNCTION public.update_modified_a_time();


--
-- Name: radiomap fk_area_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.radiomap
    ADD CONSTRAINT fk_area_id FOREIGN KEY (areaid) REFERENCES public.area(id) ON DELETE CASCADE;


--
-- Name: beacon fk_area_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.beacon
    ADD CONSTRAINT fk_area_id FOREIGN KEY (areaid) REFERENCES public.area(id) ON DELETE CASCADE;


--
-- Name: buildingmanager fk_authorization_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.buildingmanager
    ADD CONSTRAINT fk_authorization_id FOREIGN KEY (auid) REFERENCES public."authorization"(id) ON DELETE CASCADE;


--
-- Name: poimanager fk_authorization_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.poimanager
    ADD CONSTRAINT fk_authorization_id FOREIGN KEY (auid) REFERENCES public."authorization"(id) ON DELETE CASCADE;


--
-- Name: radiomapmanager fk_authorization_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.radiomapmanager
    ADD CONSTRAINT fk_authorization_id FOREIGN KEY (auid) REFERENCES public."authorization"(id) ON DELETE CASCADE;


--
-- Name: constraintmanager fk_authorization_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.constraintmanager
    ADD CONSTRAINT fk_authorization_id FOREIGN KEY (auid) REFERENCES public."authorization"(id) ON DELETE CASCADE;


--
-- Name: restreammanager fk_authorization_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.restreammanager
    ADD CONSTRAINT fk_authorization_id FOREIGN KEY (auid) REFERENCES public."authorization"(id) ON DELETE CASCADE;


--
-- Name: usermanager fk_authorization_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.usermanager
    ADD CONSTRAINT fk_authorization_id FOREIGN KEY (auid) REFERENCES public."authorization"(id) ON DELETE CASCADE;


--
-- Name: beaconmanager fk_authorization_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.beaconmanager
    ADD CONSTRAINT fk_authorization_id FOREIGN KEY (auid) REFERENCES public."authorization"(id) ON DELETE CASCADE;


--
-- Name: beaconmanager fk_beacon_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.beaconmanager
    ADD CONSTRAINT fk_beacon_id FOREIGN KEY (beaconid) REFERENCES public.beacon(id) ON DELETE CASCADE;


--
-- Name: buildingmanager fk_building_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.buildingmanager
    ADD CONSTRAINT fk_building_id FOREIGN KEY (buildingid) REFERENCES public.building(id) ON DELETE CASCADE;


--
-- Name: categorybuilding fk_building_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.categorybuilding
    ADD CONSTRAINT fk_building_id FOREIGN KEY (buildingid) REFERENCES public.building(id) ON DELETE CASCADE;


--
-- Name: constraint fk_building_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."constraint"
    ADD CONSTRAINT fk_building_id FOREIGN KEY (buildingid) REFERENCES public.building(id) ON DELETE CASCADE;


--
-- Name: floor fk_building_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.floor
    ADD CONSTRAINT fk_building_id FOREIGN KEY (buildingid) REFERENCES public.building(id) ON DELETE CASCADE;


--
-- Name: categorybuilding fk_category_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.categorybuilding
    ADD CONSTRAINT fk_category_id FOREIGN KEY (categoryid) REFERENCES public.category(id) ON DELETE CASCADE;


--
-- Name: poi fk_category_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.poi
    ADD CONSTRAINT fk_category_id FOREIGN KEY (categoryid) REFERENCES public.category(id) ON DELETE CASCADE;


--
-- Name: constraintmanager fk_constraint_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.constraintmanager
    ADD CONSTRAINT fk_constraint_id FOREIGN KEY (constraintid) REFERENCES public."constraint"(id) ON DELETE CASCADE;


--
-- Name: area fk_floor_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.area
    ADD CONSTRAINT fk_floor_id FOREIGN KEY (floorid) REFERENCES public.floor(id) ON DELETE CASCADE;


--
-- Name: poi fk_floor_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.poi
    ADD CONSTRAINT fk_floor_id FOREIGN KEY (floorid) REFERENCES public.floor(id) ON DELETE CASCADE;


--
-- Name: poimanager fk_poi_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.poimanager
    ADD CONSTRAINT fk_poi_id FOREIGN KEY (poiid) REFERENCES public.poi(id) ON DELETE CASCADE;


--
-- Name: authorization fk_product_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."authorization"
    ADD CONSTRAINT fk_product_id FOREIGN KEY (productid) REFERENCES public.product(id) ON DELETE CASCADE;


--
-- Name: radiomapmanager fk_radiomap_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.radiomapmanager
    ADD CONSTRAINT fk_radiomap_id FOREIGN KEY (radiomapid) REFERENCES public.radiomap(id) ON DELETE CASCADE;


--
-- Name: restreammanager fk_restream_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.restreammanager
    ADD CONSTRAINT fk_restream_id FOREIGN KEY (restreamid) REFERENCES public.restream(id) ON DELETE CASCADE;


--
-- Name: authorization fk_role_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."authorization"
    ADD CONSTRAINT fk_role_id FOREIGN KEY (roleid) REFERENCES public.role(id) ON DELETE CASCADE;


--
-- Name: usermanager fk_user_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.usermanager
    ADD CONSTRAINT fk_user_id FOREIGN KEY (userid) REFERENCES public."user"(id) ON DELETE CASCADE;


--
-- Name: authorization fk_user_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."authorization"
    ADD CONSTRAINT fk_user_id FOREIGN KEY (userid) REFERENCES public."user"(id) ON DELETE CASCADE;


--
-- PostgreSQL database dump complete
--

