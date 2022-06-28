drop table if exists "log";
CREATE TABLE "log" (
	id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
	message TEXT NOT NULL,
	"scope" TEXT,
	subject INTEGER,
	create_ts datetime DEFAULT current_timestamp
);

drop table if exists "session";
CREATE TABLE IF NOT EXISTS "session" (
	id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
    uuid text,
	"data" TEXT,
	create_ts datetime DEFAULT current_timestamp NOT NULL,
	update_ts datetime,
    expire_ts datetime
);

drop table if exists service;
create table if not exists "service" (
    id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
    "name" TEXT,
    "type" TEXT,
    proto TEXT,
    port integer,
    is_active boolean,
    create_ts datetime DEFAULT current_timestamp NOT NULL,
    update_ts datetime
);

drop table if exists request_response;
CREATE TABLE request_response (
	id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
    "session_id" integer,
	service_id INTEGER,
	request_bytes BLOB,
	response_bytes BLOB,
	sender TEXT,
	recipient TEXT,
	create_ts datetime DEFAULT current_timestamp,
    update_ts datetime
);

drop table if exists "envelope";
create table "envelope" (
    id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
    uuid varchar unique,
    data_bytes blob,
    create_ts datetime DEFAULT current_timestamp
);

drop table if exists "envelope_headers";
create table "envelope_headers" (
    id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
    envelope_uuid text, -- envelope.uuid
    header_key text,
    header_value text,
    create_ts datetime DEFAULT current_timestamp
);

drop table if exists "envelope_body";
create table "envelope_body" (
    id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
    uuid text,
    envelope_uuid text, -- envelope.uuid
    data_bytes blob,
    create_ts datetime DEFAULT current_timestamp
);

drop table if exists "envelope_fault";
create table "envelope_fault" (
    id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
    uuid text,
    envelope_uuid text, -- envelope.uuid
    envelope_body_uuid text, -- envelope_body.uuid
    data_bytes blob,
    create_ts datetime DEFAULT current_timestamp
);


drop table if exists "envelope_chunk";
create table "envelope_chunk" (
    id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
    envelope_uuid text, -- envelope.uuid
    start_byte_position integer,
    chunk_size integer,
    is_tranferred boolean,
    is_acknowledged_required boolean, 
    is_acknowledged boolean,
    create_ts datetime DEFAULT current_timestamp,
    transferred_ts datetime,
    acknowledged_ts datetime
);



drop table if exists "queue";
create table "queue" (
   	id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
    "session_id" integer,
    data_bytes blob,
    create_ts datetime DEFAULT current_timestamp,
    update_ts datetime
);
