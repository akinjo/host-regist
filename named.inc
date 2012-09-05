<?php

class DNS_DB { 
    var $db;
    var $ret;
    var $DOMAIN_LIST;
    
    function DNS_DB($PG_HOST,$PG_PORT,$PG_DB,$PG_USER,$PG_PASS){
	$str = sprintf("dbname=%s port=%s host=%s user=%s password=%s",
		       $PG_DB,$PG_PORT,$PG_HOST,$PG_USER,$PG_PASS);
	$this->db = pg_connect($str);    
    }

    function get(){
	$array = pg_fetch_array($this->ret);
	return($array);
    }
    
    function updateSerial($domain){
	$query = sprintf("select rdata from tbl_data where rdtype='SOA' and domain = '%s';",$domain);
	$ret   = pg_query($this->db,$query);
	$array = pg_fetch_array($ret);
	$list = array();
	$list = split("\ ",$array['rdata']);
	$serial = $list[2];
	$serial++;
	
	$rdata = sprintf("kanai.ie.u-ryukyu.ac.jp. hostmaster.ie.u-ryukyu.ac.jp. %d 28800 7200 604800 86400",$serial);
	$query = sprintf("update tbl_data set rdata = '%s' where domain = '%s' and rdtype='SOA'; ",$rdata,$domain);
	$ret   = pg_query($this->db,$query);
    }
    
    
    function getFreeIPAddr(){
	/* used = t 使用中 lease =t 貸出可能 */
	$query = sprintf("select min(ipaddr) from tbl_ipaddr where used = 'f' and lease = 'f';");
	$ret   = pg_query($this->db,$query);
	$array = pg_fetch_array($ret);

	//$ipstr = preg_replace("/\.0{1,2}/",".",$array['min']);
	$ipstr = $array['min'];
	
	/* 配布済みフラグを立てる */
	$query = sprintf("update tbl_ipaddr set used = 't', lease = 't' where ipaddr = '%s';",$array['min']);
	$ret   = pg_query($this->db,$query);
	
	return($ipstr);
    }
    
    function getDomainList($user){
	$query = "SELECT name,\"select\",mail,tbl_domain.domain ".
	    "from tbl_domain left join tbl_admin_user on tbl_domain.domain = tbl_admin_user.domain ".
            "where tbl_admin_user.edit = 't'".
	    "order by tbl_domain.id;";
	$ret   = pg_query($this->db,$query);

	$list = array();

	while($array = pg_fetch_array($ret)){
	    if(!isset($list[$array['domain']])){
		//$list[$array['domain']]['name']   = sprintf("%s (%s)",$array['name'],strtoupper($array['domain']));
		$list[$array['domain']]['name']   = $array['name'];
		$list[$array['domain']]['select'] = ($array['select'] == 't') ? true : false;
		$list[$array['domain']]['mail']   = array();
		$list[$array['domain']]['isEdit'] = false;
	    }
	    array_push($list[$array['domain']]['mail'],$array['mail']);
	}
	
	/* 管理者権限の設定 */
	$query = sprintf("select * from tbl_admin_user where sid='%s';",$user);
	$ret   = pg_query($this->db,$query);

	$list['isEdit'] = false;
	while($array = pg_fetch_array($ret)){
	    if($array['edit'] == "t"){
		$list[$array['domain']]['isEdit'] = true;
		$list['isEdit'] = true;
	    } 
	}

	return($list);
    }
    
    function getDataByID($id){
	$query = sprintf("select * from tbl_data where id = '%d';",$id);
	$ret   = pg_query($this->db,$query); 
	$array = pg_fetch_array($ret);
	return($array);
    }
    
    function queryListbyUser($user,$order ="id"){
	if(empty($user)){
	    return(NULL);
	}
	$query = sprintf("select * from tbl_data where use = 't' and rdtype = 'A' and sid = '%s' and dtime is null order by %s;",
			 $user,$order);
	$this->ret   = pg_query($this->db,$query);
    }
    
    function countbyDomain($domain="st"){
	if(empty($domain)){ $domain = "st"; }
	
	$query = sprintf("select count(*) from tbl_data".
			 "        where use = 't' and rdtype = 'A' and domain = '%s' and dtime is null; ",
			 $domain);
	$ret   = pg_query($this->db,$query);
	$array = pg_fetch_array($ret);
	return($array['count']);
    }

    function countbySIDMatch($sid){
	$query = sprintf("select count(*) from tbl_data".
			 "      where use='t'and rdtype='A' and sid='%s' and dtime is null;",$sid);
	$ret   = pg_query($this->db,$query);
	$array = pg_fetch_array($ret);
	return($array['count']);
    }
    function queryListbyDomain($domain="st",$order = "id",$page=1){
	global $NUM_RECORD;
	if(empty($domain)){
	    $domain = "st";
	}
	if($page == "ALL"){
	    $query = sprintf("select * from tbl_data".
			     "        where use = 't' and rdtype = 'A' and domain = '%s' and dtime is null".
			     "        order by %s ;",
			     $domain,$order);
	} else {
	    $query = sprintf("select * from tbl_data".
			     "        where use = 't' and rdtype = 'A' and domain = '%s' and dtime is null".
			     "        order by %s ".
			     "        offset %d limit %d;",
			     $domain,$order,($page - 1) * $NUM_RECORD,$NUM_RECORD);
	}
	$this->ret   = pg_query($this->db,$query);
    }
    
    function countbyIPAddr($ipaddr=48){
	if(empty($ipaddr)){ $ipaddr = 48;}
	$query = sprintf("select count(*) from tbl_data".
			 "        where use = 't' and rdtype = 'A' and rdata like  '133.013.%03d.%%';",
			 $ipaddr);
	$ret   = pg_query($this->db,$query);
	$array = pg_fetch_array($ret);
	return($array['count']);
 
    }
    function queryListbyIPAddr($ipaddr=48,$order = "id",$page=1){
	global $NUM_RECORD;
	if(empty($ipaddr)){ $ipaddr = 48;}

	if($page == "ALL"){
	    $query = sprintf("select * from tbl_data".
			     "        where use = 't' and rdtype = 'A' and rdata like '133.013.%03d.%%' and dtime is null".
			     "        order by %s ;",
			     $ipaddr,$order);
	} else {
	    $query = sprintf("select * from tbl_data".
			     "        where use = 't' and rdtype = 'A' and rdata like '133.013.%03d.%%' and dtime is null".
			     "        order by %s ".
			     "        offset %d limit %d;",
			     $ipaddr,$order,($page - 1) * $NUM_RECORD,$NUM_RECORD);
	}
	$this->ret   = pg_query($this->db,$query);
    }
    
    function queryCNAME($order = "id",$domain = NULL){
	$query = sprintf("SELECT * from tbl_data where use = 't' and rdtype = 'CNAME' and not domain like 'rev%%' order by %s;",$order);
	$this->ret   = pg_query($this->db,$query);
    }
    
    function queryMX($order = "id",$domain = NULL){
	$query = sprintf("SELECT * from tbl_data".
			 "   where rdtype = 'MX' and use = 't' and edit = 't' ".
			 "     and dtime is null and (not domain like 'rev%%') order by %s;",$order);
	$this->ret   = pg_query($this->db,$query);
    }
    
    function querySOA($order = "id",$domain = NULL){
	$query = sprintf("SELECT * from tbl_data where use = 't' and rdtype = 'SOA' order by %s;",$order);
	$this->ret   = pg_query($this->db,$query);
    }
    
    
    function insertARecord($_DATA){
	foreach ($_DATA as $key => $value){
	    $DATA[$key] = pg_escape_string($value);
	}
	
	$iplist   = split("\.",$DATA['ipaddr']);
	
	/* IP(v4) アドレスの新規登録 正引き(メインデータ) */
	$query = sprintf("insert into tbl_data (name,rdtype,rdata,user_name,sid,mail,domain," .
			 "                macaddr_wired,macaddr_wireless,description,ipv6,mx) ".
			 "        values ('%s','A','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s');",
			 $DATA['name'],$DATA['ipaddr'],$DATA['user_name'],$DATA['sid'],
			 $DATA['mail'],$DATA['domain'],$DATA['macaddr_wired'],$DATA['macaddr_wireless'],
			 $DATA['description'],$DATA['ipv6'],$DATA['mx']);
	$ret   = pg_query($this->db,$query);

	/* IP(v4) アドレスの新規登録 逆引き */
	$query = sprintf("insert into tbl_data (name,rdtype,rdata,domain,edit ) ".
			 "        values ('%s', 'PTR', '%s','rev_%02d','f');",
			 $DATA['iprev'],$DATA['name'],$iplist[2]);
	$ret   = pg_query($this->db,$query);
	
	/* IPv6 正引き の登録 */
	$query = sprintf("insert into tbl_data (name,rdtype,rdata,domain,use,edit)" . 
			 "       values ('%s','AAAA','%s','%s','%s','f');",
			 $DATA['name'],$DATA['ipv6addr'],$DATA['domain'],$DATA['ipv6']);
	$ret   = pg_query($this->db,$query);
	
	/* IPv6 逆引き の登録 */
	$query = sprintf("insert into tbl_data (name,rdtype,rdata,domain,use,edit)" . 
			 "       values ('%s', 'PTR', '%s','%s','%s','f');",
			 $DATA['ipv6rev'],$DATA['name'],$DATA['dom_r6'],$DATA['ipv6']);
	$ret   = pg_query($this->db,$query);

	/* MX レコードの登録 */
	$query = sprintf("insert into tbl_data (name,rdtype,rdata,domain,use,edit)" . 
			 "       values ('%s','MX','100 %s','%s','%s','f');",
			 $DATA['name'],$DATA['name'],$DATA['domain'],$DATA['mx']);
	$ret   = pg_query($this->db,$query);

	/* シリアルの更新 */
	$this->updateSerial($DATA['domain']);
	$this->updateSerial("rev_".(int)$iplist[2]);
	$this->updateSerial($DATA['dom_r6']);
    }


    function deleteARecord($DATA){
	$iplist = split("\.",$DATA['ipaddr']);
	$ipstr  = sprintf("%03d.%03d.%03d.%03d",$iplist[0],$iplist[1],$iplist[2],$iplist[3]);
	
	/* IP(v4,v6) アドレス・MXレコードの削除(正引き)*/
	$query = sprintf("update tbl_data set use = 'f' , DTIME = CURRENT_TIMESTAMP where name = '%s';",$DATA['name']);
	$ret   = pg_query($this->db,$query);
	
	/* IP(v4,v6) アドレスの削除(逆引き)*/
	$query = sprintf("update tbl_data set use = 'f' , DTIME = CURRENT_TIMESTAMP where rdata = '%s';",$DATA['name']);
	$ret   = pg_query($this->db,$query);

	/* IP アドレス管理 テーブルの更新 */
	$query  = sprintf("UPDATE tbl_ipaddr SET used = 'f' where ipaddr = '%s';",$ipstr);
	$ret    = pg_query($this->db,$query);

	/* シリアルの更新 */
	$this->updateSerial($DATA['domain']);
	$this->updateSerial("rev_".(int)$iplist[2]);
	$this->updateSerial("rev6_50");
    }

    function updateARecord($_DATA){
	foreach ($_DATA as $key => $value){
	    $DATA[$key] = pg_escape_string($value);
	}
	$iplist = split("\.",$DATA['ipaddr']);
	/* IP(v4) アドレスの更新 正引き */
	$query = sprintf("UPDATE tbl_data SET name = '%s', sid='%s',user_name = '%s', mail = '%s', domain = '%s',".
			 "              macaddr_wired = '%s' , macaddr_wireless = '%s',".
			 "              description = '%s' ,ipv6 = '%s' , mx = '%s', MTIME = CURRENT_TIMESTAMP " . 
			 "          where  rdtype = 'A' and name = '%s' and use='t';",
			 $DATA['name'],$DATA['sid'],$DATA['user_name'],$DATA['mail'],$DATA['domain'],
			 $DATA['macaddr_wired'],$DATA['macaddr_wireless'],
			 $DATA['description'],$DATA['ipv6'],$DATA['mx'],$DATA['name_old']);
	$ret   = pg_query($this->db,$query);
	$this->updateSerial($DATA['domain']);
	
	/* IP(v4,v6) アドレスの更新 逆引き */
	$query = sprintf("update tbl_data set rdata = '%s'".
			 "          where rdata ='%s' and domain='rev_%02d' and rdtype ='PTR';",
			 $DATA['name'],$DATA['name_old'],$iplist[2]);
	$ret   = pg_query($this->db,$query);

	
	$query = sprintf("update tbl_data set name = '%s', use = '%s' , mtime = CURRENT_TIMESTAMP".
			 "  where rdtype='AAAA' and dtime is null and name = '%s';",
			 $DATA['name'],$DATA['ipv6'],$DATA['name_old']);
	$ret   = pg_query($this->db,$query);
		
	
	$query = sprintf("update tbl_data set rdata = '%s',use = '%s', mtime = CURRENT_TIMESTAMP".
			 "  where rdtype='PTR' and domain='rev6_50' and rdata = '%s';",
			 $DATA['name'],$DATA['ipv6'],$DATA['name_old']);
	$ret   = pg_query($this->db,$query);
	
	$query = sprintf("update tbl_data set name = '%s',rdata = '100 %s',use = '%s', mtime = CURRENT_TIMESTAMP".
			 "  where rdtype='MX' and name = '%s';",
			 $DATA['name'],$DATA['name'],$DATA['mx'],$DATA['name_old']);
	$ret   = pg_query($this->db,$query);

	$this->updateSerial($DATA['domain']);
	$this->updateSerial("rev_".(int)$iplist[2]);
	$this->updateSerial($DATA['dom_r6']);
    }

    function updateAlert($id){
	$query = sprintf("update tbl_data set alert = not alert where id = %d;",$id);
	$ret   = pg_query($this->db,$query);
    }

    function updateMXRecord($_DATA){
	foreach ($_DATA as $key => $value){
	    $DATA[$key] = pg_escape_string($value);
	}
	$query = sprintf("update tbl_data SET name = '%s',rdata = '%s',domain = '%s',description = '%s' where id = %s ;",
			 $DATA['name'],$DATA['rdata'],$DATA['domain'],$DATA['description'],$DATA['id']);
	$ret   = pg_query($this->db,$query);
	$this->updateSerial($DATA['domain']);
    }
    
    function insertCNAMERecord($_DATA){
	foreach ($_DATA as $key => $value){
	    $DATA[$key] = pg_escape_string($value);
	}
	$query = sprintf("insert into tbl_data (name,rdtype,rdata,domain,description) ".
			 "        values ('%s','CNAME','%s','%s','%s');",
			 $DATA['name'],$DATA['rdata'],$DATA['domain'],$DATA['description']);
	$ret   = pg_query($this->db,$query);
	$this->updateSerial($DATA['domain']);
    }

    function deleteCNAMERecord($DATA){
	$query = sprintf("update tbl_data set use = 'f' , DTIME = CURRENT_TIMESTAMP where name = '%s';",$DATA['name']);
	$ret   = pg_query($this->db,$query);
	$this->updateSerial($DATA['domain']);
    }
    
    function updateCNAMERecord($_DATA){
	foreach ($_DATA as $key => $value){
	    $DATA[$key] = pg_escape_string($value);
	}
	$query = sprintf("update tbl_data SET name = '%s',rdata = '%s',domain = '%s',description = '%s' where id = %s ;",
			 $DATA['name'],$DATA['rdata'],$DATA['domain'],$DATA['description'],$DATA['id']);
	
	$ret   = pg_query($this->db,$query);

	$this->updateSerial($DATA['domain']);
    }

    function isExistHostName($host,$id = NULL){
	$query = sprintf("select count(*) from tbl_data ".
			 "where name ='%s' and id != '%d' and rdtype='A' and dtime is null;",
			 $host,$id);
	$ret   = pg_query($this->db,$query);
	$array = pg_fetch_array($ret);
	if($array['count'] > 0){
	    return(true);
	} else {
	    return(false);
	}
    }

    function isExistMacAddr($macaddr,$id=NULL){
	$query = sprintf("select count(*) from tbl_data ". 
			 "   where ( macaddr_wired = '%s' or macaddr_wireless = '%s')" .
			 "           and id != '%d' and rdtype = 'A' and dtime is null ;",
			 $macaddr,$macaddr,$id);
	$ret   = pg_query($this->db,$query);
	$array = pg_fetch_array($ret);
	if($array['count'] > 0){
	    return(true);
	} else {
	    return(false);
	}
    }
    
    function getAdminDATA($sid){
	$LIST  = array();
	$query = sprintf("select * from tbl_admin_user ".
			 "   where sid = '%s';",$sid);
	$ret   = pg_query($this->db,$query);
	while($DATA = pg_fetch_array($ret)){
	    $LIST['sid']       = $sid;
	    $LIST['user_name'] = $DATA['user_name'];
	    $LIST['mail']      = $DATA['mail'];
	    $LIST['domain'][$DATA['domain']] = ($DATA['edit'] == "t" ? "on" : "off");
	}
	return($LIST);
    }
    
    function getAdminListbyUser($sid = NULL){
	$LIST  = array();
	$query = sprintf("select * from tbl_admin_user order by id;");
	$ret   = pg_query($this->db,$query);

	while($DATA = pg_fetch_array($ret)){
	    $LIST[$DATA['sid']]['name'] = $DATA['user_name'];
	    $LIST[$DATA['sid']]['mail'] = $DATA['mail'];
	    if(!isset($LIST[$DATA['sid']]['domain'])){
		$LIST[$DATA['sid']]['domain'] = array();
	    }
	    if($DATA['edit'] == "t"){
		array_push($LIST[$DATA['sid']]['domain'],$DATA['domain']);
	    }
	}
	return($LIST);
    }

    function getAdminListbyDomain(){
	$LIST  = array();

	$query = sprintf("select * from tbl_admin_user where edit = 't' order by id;");
	
	$ret   = pg_query($this->db,$query);
	
	while($DATA = pg_fetch_array($ret)){
	    if(!isset($LIST[$DATA['domain']])){
		$LIST[$DATA['domain']] = array();
	    }
	    array_push($LIST[$DATA['domain']],$DATA);
	}
	return($LIST);
    }
    
    function insertAdminUser($DATA){
	global $DOMAIN_LIST;
	foreach($DOMAIN_LIST as $key => $value){
	    if(is_array($value)){
		$query = sprintf("insert into tbl_admin_user (sid,user_name,mail,domain,edit) ".
				 "  values( '%s','%s','%s','%s','%s');",
				 $DATA['sid'],$DATA['user_name'],$DATA['mail'],$key,
				 (isset($DATA['domain'][$key]) && $DATA['domain'][$key] == "on") ? "t" : "f");
		$ret   = pg_query($this->db,$query);
		
	    }
	}
    }

    function updateAdminUser($DATA){
	global $DOMAIN_LIST;
	$query = sprintf("update tbl_admin_user SET user_name = '%s', mail = '%s' ".
			 "    where sid = '%s';",$DATA['user_name'],$DATA['mail'],$DATA['sid']);
	$ret   = pg_query($this->db,$query);
	foreach($DOMAIN_LIST as $key => $value){
	    if(is_array($value)){
		$query = sprintf("update tbl_admin_user SET edit = '%s' ".
				 "    where sid='%s' and domain='%s';",
				 (isset($DATA['domain'][$key]) && $DATA['domain'][$key] == "on") ? "t" : "f",
				 $DATA['sid'],$key);
		$ret   = pg_query($this->db,$query);
	    }
	}
    }
    
    
    function countAdminUser($sid){
	$query = sprintf("select count(*) from tbl_admin_user ".
			 "   where sid='%s';",$sid);
	$ret   = pg_query($this->db,$query);
	$array = pg_fetch_array($ret);
	return($array['count']);
    }
}
?>
