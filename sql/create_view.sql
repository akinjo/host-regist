create view ie as SELECT name,ttl,rdtype,regexp_replace(rdata,'[.]0{0,2}','.','g') as rdata  from tbl_data where domain = 'ie' and use = 't' and dtime is null; 
create view st as SELECT name,ttl,rdtype,regexp_replace(rdata,'[.]0{0,2}','.','g') as rdata  from tbl_data where domain = 'st' and use = 't' and dtime is null; 
create view jm as SELECT name,ttl,rdtype,regexp_replace(rdata,'[.]0{0,2}','.','g') as rdata  from tbl_data where domain = 'jm' and use = 't' and dtime is null; 
create view cs as SELECT name,ttl,rdtype,regexp_replace(rdata,'[.]0{0,2}','.','g') as rdata  from tbl_data where domain = 'cs' and use = 't' and dtime is null; 
create view pc as SELECT name,ttl,rdtype,regexp_replace(rdata,'[.]0{0,2}','.','g') as rdata  from tbl_data where domain = 'pc' and use = 't' and dtime is null; 
create view iip as SELECT name,ttl,rdtype,regexp_replace(rdata,'[.]0{0,2}','.','g') as rdata  from tbl_data where domain = 'iip' and use = 't' and dtime is null; 
create view dsp as SELECT name,ttl,rdtype,regexp_replace(rdata,'[.]0{0,2}','.','g') as rdata  from tbl_data where domain = 'dsp' and use = 't' and dtime is null; 
create view eva as SELECT name,ttl,rdtype,regexp_replace(rdata,'[.]0{0,2}','.','g') as rdata  from tbl_data where domain = 'eva' and use = 't' and dtime is null; 
create view nal as SELECT name,ttl,rdtype,regexp_replace(rdata,'[.]0{0,2}','.','g') as rdata  from tbl_data where domain = 'nal' and use = 't' and dtime is null; 
create view tea as SELECT name,ttl,rdtype,regexp_replace(rdata,'[.]0{0,2}','.','g') as rdata  from tbl_data where domain = 'tea' and use = 't' and dtime is null; 
create view lsi as SELECT name,ttl,rdtype,regexp_replace(rdata,'[.]0{0,2}','.','g') as rdata  from tbl_data where domain = 'lsi' and use = 't' and dtime is null; 
create view engr as SELECT name,ttl,rdtype,regexp_replace(rdata,'[.]0{0,2}','.','g') as rdata  from tbl_data where domain = 'engr' and use = 't' and dtime is null; 
create view cr as SELECT name,ttl,rdtype,regexp_replace(rdata,'[.]0{0,2}','.','g') as rdata  from tbl_data where domain = 'cr' and use = 't' and dtime is null; 
create view fts as SELECT name,ttl,rdtype,regexp_replace(rdata,'[.]0{0,2}','.','g') as rdata  from tbl_data where domain = 'fts' and use = 't' and dtime is null; 
create view nc as SELECT name,ttl,rdtype,regexp_replace(rdata,'[.]0{0,2}','.','g') as rdata  from tbl_data where domain = 'nc' and use = 't' and dtime is null; 
create view neo as SELECT name,ttl,rdtype,regexp_replace(rdata,'[.]0{0,2}','.','g') as rdata  from tbl_data where domain = 'neo' and use = 't' and dtime is null; 
create view ads as SELECT name,ttl,rdtype,regexp_replace(rdata,'[.]0{0,2}','.','g') as rdata  from tbl_data where domain = 'ads' and use = 't' and dtime is null; 
create view ms as SELECT name,ttl,rdtype,regexp_replace(rdata,'[.]0{0,2}','.','g') as rdata  from tbl_data where domain = 'ms' and use = 't' and dtime is null; 
create view sys as SELECT name,ttl,rdtype,regexp_replace(rdata,'[.]0{0,2}','.','g') as rdata  from tbl_data where domain = 'sys' and use = 't' and dtime is null; 
create view rev_48 as SELECT name,ttl,rdtype,regexp_replace(rdata,'[.]0{0,2}','.','g') as rdata  from tbl_data where domain = 'rev_48' and use = 't' and dtime is null; 
create view rev_49 as SELECT name,ttl,rdtype,regexp_replace(rdata,'[.]0{0,2}','.','g') as rdata  from tbl_data where domain = 'rev_49' and use = 't' and dtime is null; 
create view rev_50 as SELECT name,ttl,rdtype,regexp_replace(rdata,'[.]0{0,2}','.','g') as rdata  from tbl_data where domain = 'rev_50' and use = 't' and dtime is null; 
create view rev_51 as SELECT name,ttl,rdtype,regexp_replace(rdata,'[.]0{0,2}','.','g') as rdata  from tbl_data where domain = 'rev_51' and use = 't' and dtime is null; 
create view rev_52 as SELECT name,ttl,rdtype,regexp_replace(rdata,'[.]0{0,2}','.','g') as rdata  from tbl_data where domain = 'rev_52' and use = 't' and dtime is null; 
create view rev_53 as SELECT name,ttl,rdtype,regexp_replace(rdata,'[.]0{0,2}','.','g') as rdata  from tbl_data where domain = 'rev_53' and use = 't' and dtime is null; 
create view rev_54 as SELECT name,ttl,rdtype,regexp_replace(rdata,'[.]0{0,2}','.','g') as rdata  from tbl_data where domain = 'rev_54' and use = 't' and dtime is null; 
create view rev_55 as SELECT name,ttl,rdtype,regexp_replace(rdata,'[.]0{0,2}','.','g') as rdata  from tbl_data where domain = 'rev_55' and use = 't' and dtime is null; 
create view rev_56 as SELECT name,ttl,rdtype,regexp_replace(rdata,'[.]0{0,2}','.','g') as rdata  from tbl_data where domain = 'rev_56' and use = 't' and dtime is null; 
create view rev_57 as SELECT name,ttl,rdtype,regexp_replace(rdata,'[.]0{0,2}','.','g') as rdata  from tbl_data where domain = 'rev_57' and use = 't' and dtime is null; 
create view rev_58 as SELECT name,ttl,rdtype,regexp_replace(rdata,'[.]0{0,2}','.','g') as rdata  from tbl_data where domain = 'rev_58' and use = 't' and dtime is null; 
create view rev_59 as SELECT name,ttl,rdtype,regexp_replace(rdata,'[.]0{0,2}','.','g') as rdata  from tbl_data where domain = 'rev_59' and use = 't' and dtime is null; 
create view rev_60 as SELECT name,ttl,rdtype,regexp_replace(rdata,'[.]0{0,2}','.','g') as rdata  from tbl_data where domain = 'rev_60' and use = 't' and dtime is null; 
create view rev_61 as SELECT name,ttl,rdtype,regexp_replace(rdata,'[.]0{0,2}','.','g') as rdata  from tbl_data where domain = 'rev_61' and use = 't' and dtime is null; 
create view rev_62 as SELECT name,ttl,rdtype,regexp_replace(rdata,'[.]0{0,2}','.','g') as rdata  from tbl_data where domain = 'rev_62' and use = 't' and dtime is null; 
create view rev_63 as SELECT name,ttl,rdtype,regexp_replace(rdata,'[.]0{0,2}','.','g') as rdata  from tbl_data where domain = 'rev_63' and use = 't' and dtime is null; 
create view rev6_48 as SELECT name,ttl,rdtype,regexp_replace(rdata,'[.]0{0,2}','.','g') as rdata  from tbl_data where domain = 'rev6_48' and use = 't' and dtime is null; 
create view rev6_49 as SELECT name,ttl,rdtype,regexp_replace(rdata,'[.]0{0,2}','.','g') as rdata  from tbl_data where domain = 'rev6_49' and use = 't' and dtime is null; 
create view rev6_4a as SELECT name,ttl,rdtype,regexp_replace(rdata,'[.]0{0,2}','.','g') as rdata  from tbl_data where domain = 'rev6_4a' and use = 't' and dtime is null; 
create view rev6_4b as SELECT name,ttl,rdtype,regexp_replace(rdata,'[.]0{0,2}','.','g') as rdata  from tbl_data where domain = 'rev6_4b' and use = 't' and dtime is null; 
create view rev6_4c as SELECT name,ttl,rdtype,regexp_replace(rdata,'[.]0{0,2}','.','g') as rdata  from tbl_data where domain = 'rev6_4c' and use = 't' and dtime is null; 
create view rev6_4d as SELECT name,ttl,rdtype,regexp_replace(rdata,'[.]0{0,2}','.','g') as rdata  from tbl_data where domain = 'rev6_4d' and use = 't' and dtime is null; 
create view rev6_4e as SELECT name,ttl,rdtype,regexp_replace(rdata,'[.]0{0,2}','.','g') as rdata  from tbl_data where domain = 'rev6_4e' and use = 't' and dtime is null; 
create view rev6_4f as SELECT name,ttl,rdtype,regexp_replace(rdata,'[.]0{0,2}','.','g') as rdata  from tbl_data where domain = 'rev6_4f' and use = 't' and dtime is null; 
create view rev6_50 as SELECT name,ttl,rdtype,regexp_replace(rdata,'[.]0{0,2}','.','g') as rdata  from tbl_data where domain = 'rev6_50' and use = 't' and dtime is null; 