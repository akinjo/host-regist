<?php
include_once("./ini.inc");
include_once("./named.inc");
include_once("./func.inc");
include_once("./print.inc");
include_once("./ldap.inc");
include_once("./error.inc");

if( !isset($_POST['cmd'] )) {$_SESSION = array();}
$_SESSION['PHP_AUTH_USER'] = $_SERVER['PHP_AUTH_USER'];
$DNS = new DNS_DB($PG_HOST,$PG_PORT,$PG_DB,$PG_USER,$PG_PASS);
$DOMAIN_LIST = $DNS->getDomainList($_SERVER['PHP_AUTH_USER']);

$LDAP = new LDAP($LDAP_SERVER,$LDAP_BASE_DN,$LDAP_BIND_DN,$LDAP_PASS);
$LDAP_DATA = $LDAP->getUserDATA($_SERVER['PHP_AUTH_USER']);

$ERROR = array();

if(isset($_POST['cmd']) && $_POST['cmd'] == "regist"){
    checkAdminUser($ERROR,$_POST,$DNS,$LDAP);
    if(empty($ERROR)){
	if(isset($_GET['cmd']) && $_GET['cmd'] == "new"){
	    $DNS->insertAdminUser($_POST);
	} else if(isset($_GET['cmd']) && $_GET['cmd'] == "edit"){
	    $DNS->updateAdminUser($_POST);
	}
    }
}

?>
<!DOCTYPE HTML PUBLIC "-//IETF//DTD HTML//EN">
<html>
<head>
<meta http-equiv="Content-Type"  content="text/html;charset=UTF-8" />
<meta http-equiv="Cache-control" content="no-cache" />
<meta http-equive="Pragma" content="no-cache" />
<link rel="stylesheet" href="./style.css" type="text/css" />
<title>情報工学科 IP申請システム 管理者登録ページ</title>
</head>
<body>

<h1 id="index_title">IP Register </h1>
<h3 id="summary"><?php print $LDAP_DATA['cn'][0]; ?> さん<br />情報工学科 IP申請システム 管理者登録ページへようこそ</h3>
<hr style="width: 100%;">


<div id="menu">
<h3>メニュー</h3>
[<a href="<?php print $_SERVER['SCRIPT_NAME']; ?>">トップページ</a>]
[<a href="<?php print $_SERVER['SCRIPT_NAME']; ?>?cmd=new">新規登録</a>]
[<a href="<?php print $_SERVER['SCRIPT_NAME']; ?>?list=domain">ドメイン別一覧</a>]
[<a href="<?php print $_SERVER['SCRIPT_NAME']; ?>?list=user">管理者別一覧</a>]
</div>
<hr style="width: 100%;">
<div id="body">
<?php
if(isset($_GET['cmd']) && $_GET['cmd'] == "new"){
    if($DOMAIN_LIST['ie']['isEdit'] == false){
	h3("登録する権限がありません。");
    }else if(isset($_POST['cmd']) && $_POST['cmd'] == "regist" && empty($ERROR)){
	h3("登録が完了しました。");
    } else {
	printAdminUserEditForm($_POST,$ERROR);
    }
} else if(isset($_GET['cmd']) && $_GET['cmd'] == "edit"){
    if($DOMAIN_LIST['ie']['isEdit'] == false){
	h3("登録する権限がありません。");
    } else if(isset($_POST['cmd']) && $_POST['cmd'] == "regist" && empty($ERROR)){
	h3("更新が完了しました。");
    } else {
	if(isset($_POST['cmd']) && $_POST['cmd'] == "regist"){
	    printAdminUserEditForm($_POST,$ERROR,"edit");
	} else {
	    $DATA  = $DNS->getAdminDATA($_POST['sid']);
	    printAdminUserEditForm($DATA,$ERROR,"edit");
	}
    }
} else if(isset($_GET['list']) && $_GET['list'] == "user"){
    printAdminListbyUser($DNS);
} else {
    printAdminListbyDomain($DNS);
}

?>
</div>

</body>
</html>