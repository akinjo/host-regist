<?php
header("Content-type: text/plain"); 
include_once("./func.inc");

$PG_HOST = "133.13.48.7";
$PG_PORT = "5432";
$PG_DB   = "bind";
$PG_USER = "named"; 
$PG_PASS = "root58330";

$DOMAIN_LIST = array(
    "ie"   => "ie.u-ryukyu.ac.jp",
    "cs"   => "cs.ie.u-ryukyu.ac.jp",
    "st"   => "st.ie.u-ryukyu.ac.jp",
    "pc"   => "pc.ie.u-ryukyu.ac.jp",
    "iip"  => "iip.ie.u-ryukyu.ac.jp",
    "dsp"  => "dsp.ie.u-ryukyu.ac.jp",
    "eva"  => "eva.ie.u-ryukyu.ac.jp",
    "nal"  => "nal.ie.u-ryukyu.ac.jp",
    "tea"  => "tea.ie.u-ryukyu.ac.jp",
    "lsi"  => "lsi.ie.u-ryukyu.ac.jp",
    "engr" => "engr.ie.u-ryukyu.ac.jp",
    "fts"  => "fts.ie.u-ryukyu.ac.jp",
    "cr"   => "cr.ie.u-ryukyu.ac.jp",
    "jm"   => "jm.ie.u-ryukyu.ac.jp",
    "neo"  => "neo.ie.u-ryukyu.ac.jp",
    "ads"  => "ads.ie.u-ryukyu.ac.jp",
    "ms"   => "ms.ie.u-ryukyu.ac.jp",
    "sys"  => "sys.ie.u-ryukyu.ac.jp",
    "nc"   => "nc.ie.u-ryukyu.ac.jp",
    "rev_48" => "48.13.133.in-addr.arpa",
    "rev_49" => "49.13.133.in-addr.arpa",
    "rev_50" => "50.13.133.in-addr.arpa",
    "rev_51" => "51.13.133.in-addr.arpa",
    "rev_52" => "52.13.133.in-addr.arpa",
    "rev_53" => "53.13.133.in-addr.arpa",
    "rev_54" => "54.13.133.in-addr.arpa",
    "rev_55" => "55.13.133.in-addr.arpa",
    "rev_56" => "56.13.133.in-addr.arpa",
    "rev_57" => "57.13.133.in-addr.arpa",
    "rev_58" => "58.13.133.in-addr.arpa",
    "rev_59" => "59.13.133.in-addr.arpa",
    "rev_60" => "60.13.133.in-addr.arpa",
    "rev_61" => "61.13.133.in-addr.arpa",
    "rev_62" => "62.13.133.in-addr.arpa",
    "rev_63" => "63.13.133.in-addr.arpa",
    "rev6_48" => "8.4.0.d.c.1.0.0.8.f.2.0.1.0.0.2.ip6.arpa",
    "rev6_49" => "9.4.0.d.c.1.0.0.8.f.2.0.1.0.0.2.ip6.arpa",
    "rev6_4a" => "a.4.0.d.c.1.0.0.8.f.2.0.1.0.0.2.ip6.arpa",
    "rev6_4b" => "b.4.0.d.c.1.0.0.8.f.2.0.1.0.0.2.ip6.arpa",
    "rev6_4c" => "c.4.0.d.c.1.0.0.8.f.2.0.1.0.0.2.ip6.arpa",
    "rev6_4d" => "d.4.0.d.c.1.0.0.8.f.2.0.1.0.0.2.ip6.arpa",
    "rev6_4e" => "e.4.0.d.c.1.0.0.8.f.2.0.1.0.0.2.ip6.arpa",
    "rev6_4f" => "f.4.0.d.c.1.0.0.8.f.2.0.1.0.0.2.ip6.arpa",
    "rev6_50" => "0.5.0.d.c.1.0.0.8.f.2.0.1.0.0.2.ip6.arpa" );


foreach ($DOMAIN_LIST as $key => $value){
    printf("insert into tbl_data (name,rdtype,rdata,domain) ".
	   "  values ('%s','SOA','kanai.ie.u-ryukyu.ac.jp. hostmaster.ie.u-ryukyu.ac.jp. %s00 28800 7200 604800 86400','%s');\n",
	   $value,date("Ymd"),$key);
    printf("insert into tbl_data (name,rdtype,rdata,domain) ".
	   "  values ('%s','NS','kanai.ie.u-ryukyu.ac.jp.','%s');\n",
	   $value,$key);
    printf("insert into tbl_data (name,rdtype,rdata,domain) ".
	   "  values ('%s','NS','nirai.ie.u-ryukyu.ac.jp.','%s');\n",
	   $value,$key);
}

$str = sprintf("dbname=%s port=%s host=%s user=%s password=%s",
	       $PG_DB,$PG_PORT,$PG_HOST,$PG_USER,$PG_PASS);
$db = pg_connect($str);

/* TBL MX */
$query = "select * from base where rdtype='MX' order by id;";
$ret = pg_query($db,$query);

while($DATA = pg_fetch_array($ret)){
    $query = sprintf("insert into tbl_data (name,rdtype,rdata,domain) ".
		     " values ('%s','MX','%s','%s'); ",
		     $DATA['name'],$DATA['rdata'],$DATA['domain']);
    print $query ."\n";
}

$query = "select * from base where rdtype='CNAME' order by id;";
$ret = pg_query($db,$query);

while($DATA = pg_fetch_array($ret)){
    $query = sprintf("insert into tbl_data (name,rdtype,rdata,domain) ".
		     " values ('%s','CNAME','%s','%s'); ",
		     $DATA['name'],$DATA['rdata'],$DATA['domain']);
    print $query ."\n";
}
    

/* TBL DATA */
$query = "select * from base where rdtype='A' order by id;";
$ret = pg_query($db,$query);

while($DATA = pg_fetch_array($ret)){
    $iplist = split("\.",$DATA['rdata']);
    $DATA['ipaddr'] = sprintf("%03d.%03d.%03d.%03d",$iplist[0],$iplist[1],$iplist[2],$iplist[3]);
    setIPAddr($DATA);

    /* IP(v4) アドレスの新規登録 正引き(メインデータ) */
    $query = sprintf("insert into tbl_data (name,rdtype,rdata,user_name,sid,mail,domain,macaddr_wired,macaddr_wireless,description,use,ipv6,atime,ctime,mtime,dtime)".
		     " values ('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s',%s);",
		     $DATA['name'],$DATA['rdtype'],$DATA['ipaddr'],$DATA['user_name'],$DATA['sid'],$DATA['mail'],$DATA['domain'],
		     $DATA['macaddr_wired'],$DATA['macaddr_wireless'],
		     $DATA['remarks'],$DATA['display'],$DATA['ipv6'],
		     $DATA['ts'],$DATA['ts'],$DATA['ts'],
		     $DATA['display'] == "f" ? "now()" : "null");
    
    print $query ."\n";
    /* IP(v4) アドレスの新規登録 逆引き */
    $query = sprintf("insert into tbl_data (name,rdtype,rdata,domain,use,edit,dtime)".
		     " values ('%s', 'PTR', '%s','rev_%02d','t','f',%s);",
		     $DATA['iprev'],$DATA['name'],$iplist[2],
		     $DATA['display'] == "f" ? "now()" : "null");
    print $query ."\n";
    /* IPv6 正引き の登録 */
    $query = sprintf("insert into tbl_data (name,rdtype,rdata,domain,use,edit,dtime)" . 
		     " values ('%s','AAAA','%s','%s','%s','f',%s);",
		     $DATA['name'],$DATA['ipv6addr'],$DATA['domain'],$DATA['ipv6'],
		     $DATA['display'] == "f" ? "now()" : "null");
    print $query ."\n";
    
    /* IPv6 逆引き の登録 */
    $query = sprintf("insert into tbl_data (name,rdtype,rdata,domain,use,edit,dtime)" . 
		     " values ('%s', 'PTR', '%s','rev6_50','%s','f',%s);",
		     $DATA['ipv6rev'],$DATA['name'],$DATA['ipv6'],
		     $DATA['display'] == "f" ? "now()" : "null");
    
    print $query ."\n";
    /* MX レコードの登録 */
    $query = sprintf("insert into tbl_data (name,rdtype,rdata,domain,use,edit,dtime)" . 
		     " values ('%s','MX','100 %s','%s','f','f',%s);",
		     $DATA['name'],$DATA['name'],$DATA['domain'],
		     $DATA['display'] == "f" ? "now()" : "null");
    print $query ."\n";
}

$query = "select * from base where rdtype='AAAA' order by id;";
$ret = pg_query($db,$query);

while($DATA = pg_fetch_array($ret)){
    $query = sprintf("update tbl_data set ipv6 = 't' where rdtype='A' and name='%s';",
		     $DATA['name']);
    print $query ."\n";
    $query = sprintf("update tbl_data set use = 't' where rdtype='AAAA' and name='%s';",
		     $DATA['name']);
    print $query ."\n";
    $query = sprintf("update tbl_data set use = 't' where rdtype='PTR' and rdata='%s';",
		     $DATA['name']);
    print $query ."\n";
}


/* TBL IP ADDR */
for($i = 48;$i <=63;$i++){
    for($j = 0;$j <256;$j++){
	$ipstr = sprintf("%03d.%03d.%03d.%03d",
			 133,13,$i,$j);

        if($i <= 55 || $j == 0 || $j == 255 || $j == 254){
          $lease = "t";
        } else { 
          $lease = "f";
        }
	
	$query = sprintf("insert into tbl_ipaddr (ipaddr,lease,used) ".
			 " values('%s','%s','f');",
			 $ipstr,$lease);
	print $query . "\n";
    }
}

$query = sprintf("select * from ipaddr order by id;");
$ret = pg_query($db,$query);

while($DATA = pg_fetch_array($ret)){
    $ip_list = split("\.",long2ip($DATA['ipaddr']));
    $ipstr = sprintf("%03d.%03d.%03d.%03d",$ip_list[0],$ip_list[1],$ip_list[2],$ip_list[3]);
    $query = sprintf("update tbl_ipaddr set used = '%s',lease = '%s' where ipaddr = '%s';",
		     $DATA['used'],$DATA['used'],$ipstr);
    print $query . "\n";
}

?>
