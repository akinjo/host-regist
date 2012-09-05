<?php
session_name("IPREGIST");
session_start();

include_once("./named.inc");
include_once("./ldap.inc");
include_once("./dhcp.inc");
include_once("./print.inc");
include_once("./func.inc");
include_once("./error.inc");
include_once("./mail.inc");

if( empty($_SERVER['PHP_AUTH_USER'])){
    die("Could not get user id. Contact with administrator.");
}

/* 新規登録の時はセッションを削除 */
if( !isset($_POST['cmd'] )) {$_SESSION = array();}
$_SESSION['PHP_AUTH_USER'] = $_SERVER['PHP_AUTH_USER'];

/* オブジェクトの初期化 */
$DNS = new DNS_DB($PG_HOST,$PG_PORT,$PG_DB,$PG_USER,$PG_PASS);
$DOMAIN_LIST = $DNS->getDomainList($_SERVER['PHP_AUTH_USER']);

$LDAP = new LDAP($LDAP_SERVER,$LDAP_BASE_DN,$LDAP_BIND_DN,$LDAP_PASS);
$LDAP_DATA = $LDAP->getUserDATA($_SERVER['PHP_AUTH_USER']);

$DHCP = new DHCP($DHCP_SERVER,$DHCP_PORT);

$ERROR = array();
$DATA  = array(); 

readDATA($DNS,$DATA,$LDAP_DATA);
copyArray($_SESSION,$DATA);
if(isset($_POST['cmd']) && 
   ($_POST['cmd'] == "confirm" || $_POST['cmd'] == "regist")){
    checkError($ERROR,$DATA,$DNS,$LDAP);
}
cmdExec($DNS,$DHCP,$DATA);

?>
<!DOCTYPE HTML PUBLIC "-//IETF//DTD HTML//EN">
<html>
<head>
<meta http-equiv="Content-Type"  content="text/html;charset=UTF-8" />
<meta http-equiv="Cache-control" content="no-cache" />
<meta http-equive="Pragma" content="no-cache" />
<link rel="stylesheet" href="./style.css" type="text/css" />
<title><?php print $title; ?></title>
</head>
<body>

<h1 id="index_title">IP Register </h1>
<h3 id="summary"><?php print $LDAP_DATA['cn'][0]; ?> さん<br /> 琉球大学 情報工学科 IPアドレス申請ページへようこそ</h3>
<hr style="width: 100%;">

<table>
 <tr>
  <td style="vertical-align: top;">

<div id="menu">
<h3>ユーザメニュー</h3>
<ul>
 <li><a href="<?php print $_SERVER['SCRIPT_NAME']; ?>">トップページ</a></li>
 <li><a href="<?php print $_SERVER['SCRIPT_NAME']; ?>?type=A&cmd=new">新規登録</a></li>
 <li><a href="<?php print $_SERVER['SCRIPT_NAME']; ?>?type=A&cmd=list">登録一覧（編集・削除）</a></li>
</ul>

<h3>管理者メニュー</h3>
<ul>
 <li><a href="<?php print $_SERVER['SCRIPT_NAME']; ?>?type=CNAME&cmd=new">新規登録(CNAME)</a></li>
 <li><a href="<?php print $_SERVER['SCRIPT_NAME']; ?>?type=CNAME&cmd=list">登録一覧(CNAME)</a></li>
 <li><a href="<?php print $_SERVER['SCRIPT_NAME']; ?>?type=MX&cmd=list">登録一覧(MX)</a></li>
 <li><a href="<?php print $_SERVER['SCRIPT_NAME']; ?>?type=SOA&cmd=list">登録一覧(SOA)</a></li>
</ul>

<h3>所属別登録一覧</h3>
<ul>
<?php
foreach ($DOMAIN_LIST as $key => $value) {
    if(isset($value['name'])){
	printf(" <li><a href=\"%s?type=A&cmd=list&domain=%s\">%s(%s)</a></li>\n",
	       $_SERVER['SCRIPT_NAME'],$key,$value['name'],strtoupper($key));
    }
}
?>
</ul>

<h3>IPアドレス別一覧</h3>
<ul>
<?php
for($i=48;$i<64;$i++){
    printf(" <li><a href=\"%s?type=A&cmd=list&ipaddr=%d\">133.13.%d.0-255</a></li>\n",
	   $_SERVER['SCRIPT_NAME'],$i,$i);
}
?>
</ul>
</div>
  </td>

  <td style="vertical-align: top;">

<div id="body">
<?php
$func = (isset($_GET['type']) ? $_GET['type'] : "") . "Body";
if(function_exists($func)){
    if(!call_user_func_array($func,array($DNS,$DATA,$ERROR))){
	h3("登録データ一覧");	
	printListbyUser($DNS,$_SERVER['PHP_AUTH_USER'],true);
    }
} else {
    h3("登録データ一覧");
    printListbyUser($DNS,$_SERVER['PHP_AUTH_USER'],true);
}
/*
print "<pre>\n";
print_r($ERROR);
print "</pre>\n";
*/
?>

</div>
  </td>
 </tr>
</table>
<pre>
</pre>
</body>
</html>
