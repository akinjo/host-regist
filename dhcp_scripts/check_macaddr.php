<?php
include_once("./ini.inc");
include_once("./db.inc");

$macaddr_pattern = "/^[0-9a-fA-F]{2}\:[0-9a-fA-F]{2}\:[0-9a-fA-F]{2}\:[0-9a-fA-F]{2}\:[0-9a-fA-F]{2}\:[0-9a-fA-F]{2}$/";

$db = new PGSQL_DB();
$query = sprintf("select name,rdata,macaddr_wired from %s".
		 "    where display = 't' and rdtype = 'A' and macaddr_wired != '' and rdata > '133.13.55' ".
		 "    order by rdata; ",
                 $PG_TBL);
$db->query($query);

while($array = $db->get()){
  if (!preg_match($macaddr_pattern,$array['macaddr_wired'])){
    printf("%s,%s,%s\n",$array['name'],$array['rdata'],$array['macaddr_wired']);
  }
  /*
  $host = str_replace(".ie.u-ryukyu.ac.jp","",$array['name']);
  printf("host %s {\n",$host);
  printf("  dynamic;\n");
  printf("  hardware ethernet %s;\n",$array['macaddr_wired']);
  printf("  fixed-address %s;\n",$array['rdata']);
  printf("}\n");
  */
}

?>
