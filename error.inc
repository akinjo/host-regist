<?php
include_once("./named.inc");
include_once("./ldap.inc");

function checkError(&$ERROR,$DATA,$DNS,$LDAP){
    $func = "check" . ($_GET['type'] ? $_GET['type'] : "") . "Record";
    
    if(function_exists($func)){
	call_user_func_array($func,array(&$ERROR,$DATA,$DNS,$LDAP));
    }
}

function checkARecord(&$ERROR,$DATA,$DNS,$LDAP){
    checkSID($DATA,$ERROR,$DNS,$LDAP);
    checkUserName($DATA,$ERROR);
    checkMailAddr($DATA,$ERROR);
    checkHostName($DATA,$ERROR,$DNS);
    checkMacAddr($DATA,$ERROR,$DNS);
}

function checkCNAMERecord(&$ERROR,$DATA,$DNS,$LDAP){
    checkHostName($DATA,$ERROR,$DNS);
    checkAlias($DATA,$ERROR,$DNS);
}

function checkMXRecord(&$ERROR,$DATA,$DNS,$LDAP){
    checkMailServer($DATA,$ERROR,$DNS);
}

function checkAdminUser(&$ERROR,$DATA,$DNS,$LDAP){
    checkSID($DATA,$ERROR,$DNS,$LDAP,"admin");
    checkUserName($DATA,$ERROR);
    checkMailAddr($DATA,$ERROR);
}

function printErrMSG($DATA,$ERROR){
    $strhost = (isset($_GET['type']) && $_GET['type'] == "CNAME") ? "別名ホスト" : "ホスト名";
    
    $ErrMsgTBL = array(
	"sid_not_exist"                => "登録アカウント" . (isset($DATA['sid']) ? $DATA['sid'] : "") . " は存在しません。登録アカウントを確認してください。",
	"user_name_null"               => "使用者名が未入力です。使用者名を入力してください。",
	"mail_null"                    => "使用者メールアドレスが未入力です。使用者メールアドレスを確認してください。",
	"mail_not_mailaddr"            => "使用者メールアドレスの記述が間違っています。使用者メールアドレスを確認してください。",
	"host_name_null"               => $strhost . "が入力です。". $strhost ."を確認してください。",
	"host_name_ja"                 => $strhost . " は[0-9a-z-]の範囲で入力してください。",
	"host_name_duplication"        => $strhost . " " . $DATA['host_name'] . " はすでに登録されています。". $strhost ."を確認してください",
	"alias_null"                   => "参照先ホスト名が未入力です。参照先ホスト名を確認してください。",
	"macaddr_eq"                   => "MACアドレス（有線）とMACアドレス（無線）が同じ値です。",
	"macaddr_wired_null"           => "MACアドレス（有線）が未入力です。MACアドレス（有線）を確認してください。",
	"macaddr_wired_not"            => "MACアドレス（有線）の記述が間違っています。00:00:00:00:00:00 の形式で入力してください",
	"macaddr_wired_duplication"    => "MACアドレス " .$DATA['macaddr_wired'] ." は既に登録されています。MACアドレス（有線）を確認してください。",
	"macaddr_wireless_not"         => "MACアドレス（無線）の記述が間違っています。00:00:00:00:00:00 の形式で入力してください",
	"macaddr_wireless_duplication" => "MACアドレス " .$DATA['macaddr_wireless'] ." は既に登録されています。MACアドレス（有線）を確認してください。",
	"macaddr_eq"                   => "MACアドレス（有線)とMACアドレス(無線)が等しいです。入力を確認してください。",
	"mailserver_null"              => "転送先ホストが未入力です。転送先ホストを確認してください",
	"mailserver_not_exist"         => "転送先ホスト " . (isset($DATA['mailserver']) ? $DATA['mailserver'] : "") . " は存在しません。",
	"not_regist"                   => "ドメイン" . $DATA['domain']  . " の管理者権限がないため登録できません。登録アカウントまたはドメインを確認してください。"
      	);
    
    h3("エラーメッセージ");
    
    print("<ul id=\"error\" >\n");
    
    foreach($ERROR as  $key => $value){
	if(isset($ErrMsgTBL[$key])){
	    printf(" <li>%s</li>\n",$ErrMsgTBL[$key]);
	}
    }
    printf("</ul>\n");
}


function checkSID($DATA,&$ERROR,$DNS,$LDAP = null,$mode = null){
    global $DOMAIN_LIST;
    if($DATA['sid'] == "admin"){
	return;
    }
    if(!$LDAP->isSIDExist($DATA['sid'])){
	$ERROR['sid'] = true;
	$ERROR['sid_not_exist'] = true;
    } else if($DATA['sid'] != $_SERVER['PHP_AUTH_USER'] &&
	      !$DOMAIN_LIST[$DATA['domain']]['isEdit'] &&
	      $mode != "admin"	){
	$ERROR['sid'] = true;
	$ERROR['host_name'] = true;
	$ERROR['not_regist'] = true;
    } else if($mode == "admin" && $DNS->countAdminuser($DATA['sid']) > 0 && 
	      isset($_GET['cmd']) && $_GET['cmd'] != "edit"){
	$ERROR['sid'] = true;
    }

}

function checkUserName($DATA,&$ERROR){
    // ユーザ名のチェック
    if(empty($DATA['user_name'])){
	$ERROR['user_name'] = true;
	$ERROR['user_name_null'] = true;
    }
}

function checkMailAddr($DATA,&$ERROR){
  // メールアドレスのチェック
    if(empty($DATA['mail'])){
	$ERROR['mail'] = true;
	$ERROR['mail_null'] = true;
    } else if(!preg_match('/^[a-zA-Z0-9_\.\-]+?@[A-Za-z0-9_\.\-]+$/',$DATA['mail'])){
	$ERROR['mail'] = true;
	$ERROR['mail_not_mailaddr']    = true;
    }
}

function checkHostName($DATA,&$ERROR,$DNS){
    if(empty($DATA['host_name'])){
	$ERROR['host_name'] = true;
	$ERROR['host_name_null'] = true;
    } else if($DATA['domain'] == "ie" ?
	      !preg_match('/^[0-9a-zA-Z-][0-9a-zA-Z-\.]+[0-9a-zA-Z-]$/',$DATA['host_name']) :
	      !preg_match('/^[0-9a-zA-Z-]+$/',$DATA['host_name'])){
	$ERROR['host_name'] = true;
	$ERROR['host_name_ja'] = true;
    } else if($DNS->isExistHostName($DATA['name'],isset($DATA['id']) ? $DATA['id'] : "")){
	$ERROR['host_name'] = true;
	$ERROR['host_name_duplication'] = true;
    }
}

function checkAlias($DATA,&$ERROR,$DNS){
    if(empty($DATA['alias'])){
	$ERROR['alias'] = true;
	$ERROR['alias_null'] = true;
    } else if($DATA['domain'] == "ie" ?
	      !preg_match('/^[0-9a-zA-Z-][0-9a-zA-Z-\.]+[0-9a-zA-Z-]$/',$DATA['alias']) :
	      !preg_match('/^[0-9a-zA-Z-]+$/',$DATA['alias'])){
	$ERROR['alias'] = true;
	$ERROR['alias_ja'] = true;
    } else if(!$DNS->isExistHostName($DATA['rdata'],0)){
	$ERROR['alias'] = true;
	$ERROR['alias_not_exist'] = true;
    }
}

function checkMailServer($DATA,&$ERROR,$DNS){
    if(empty($DATA['mailserver'])){
	$ERROR['mailserver'] = true;
	$ERROR['mailserver_null'] = true;
    } else if(!$DNS->isExistHostName($DATA['mailserver'])){
	$ERROR['mailserver'] = true;
	$ERROR['mailserver_not_exist'] = true;
    }
}

function checkMacAddr($DATA,&$ERROR,$DNS){
    $macaddr_pattern = "/^[0-9a-fA-F]{2}\:[0-9a-fA-F]{2}\:[0-9a-fA-F]{2}\:[0-9a-fA-F]{2}\:[0-9a-fA-F]{2}\:[0-9a-fA-F]{2}$/";

    // MAC アドレス（有線）のチェック
    if(empty($DATA['macaddr_wired'])){
	$ERROR['macaddr_wired'] = true;
	$ERROR['macaddr_wired_null'] = true;
    } else if(!preg_match($macaddr_pattern,$DATA['macaddr_wired'])){  
	$ERROR['macaddr_wired'] = true;
	$ERROR['macaddr_wired_not'] = true;
    } else if(!($DATA['macaddr_wired'] == "00:00:00:00:00:00") &&
	      ($DNS->isExistMacAddr($DATA['macaddr_wired'],isset($DATA['id']) ? $DATA['id'] : ""))){
	$ERROR['macaddr_wired'] = true;
	$ERROR['macaddr_wired_duplication'] = true;
    } else if(isset($DATA['macaddr_wired']) && 
	      isset($DATA['macaddr_wireless']) &&
	      $DATA['macaddr_wired'] == $DATA['macaddr_wired']){
      $ERROR['macaddr_wired'] = true;
      $ERROR['macaddr_eq'] = true;
    }
    // MAC アドレス（無線）のチェック
    if(!empty($DATA['macaddr_wireless'])){
	if(!preg_match($macaddr_pattern,$DATA['macaddr_wireless'])){
	    $ERROR['macaddr_wireless'] = true;
	    $ERROR['macaddr_wireless_not'] = true;
	}else if($DNS->isExistMacAddr($DATA['macaddr_wireless'],isset($DATA['id']) ? $DATA['id'] : "")){
	    $ERROR['macaddr_wireless'] = true;
	    $ERROR['macaddr_wireless_duplication'] = true;
	}
    }
}

?>
