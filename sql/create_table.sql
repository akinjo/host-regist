CREATE TABLE tbl_data ( 
  ID               SERIAL primary key,
  NAME             TEXT, 
  TTL              INTEGER not null default '86500',  
  RDTYPE           TEXT,
  RDATA            TEXT,
  USER_NAME        TEXT,
  SID              TEXT,
  MAIL             TEXT,
  DOMAIN           varchar(10),
  MACADDR_WIRED    varchar(17),
  MACADDR_WIRELESS varchar(17),
  DESCRIPTION      TEXT,
  IPV6             BOOL not null default 'f',
  MX               BOOL not null default 'f',
  atime            TIMESTAMP not null default now(),
  ctime            TIMESTAMP not null default now(),
  mtime            TIMESTAMP not null default now(),
  dtime            TIMESTAMP,
  USE              BOOL not null default 't',
  ALERT            BOOL not null default 'f',
  EDIT             BOOL not null default 't',
  DELETE           BOOL not null default 't'
);

CREATE TABLE TBL_IPADDR ( 
  ID               serial primary key,
  IPADDR           varchar(15) not null,
  DOMAIN           varchar(10) ,
  LEASE            bool not null default 'f',
  USED             bool not null default 'f'
);

CREATE TABLE TBL_ADMIN_USER ( 
  ID               SERIAL primary key,
  USER_NAME        TEXT not null,
  SID              TEXT not null,
  MAIL             TEXT not null,
  DOMAIN           varchar(10) not null,
  edit             bool not null default false,
  TS               TIMESTAMP not null default now()
);

CREATE TABLE TBL_DOMAIN (
  ID               SERIAL primary key,
  DOMAIN           varchar(10) not null,
  NAME             TEXT not null,
  DESCRIPTION      TEXT,
  DISPLAY          BOOL not null default 't',
  "select"         BOOL not null default 't'
);

