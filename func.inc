<?php
include_once("ini.inc");
include_once("mail.inc");
include_once("dhcp.inc");

function h3($str){
    printf("<h3>%s</h3>\n",$str);
}

function p($str){
    printf("<p>\n%s</p>\n",$str);
}

function genHash($len) {
    $str = "";
    for($i = 0 ;$i < $len;$i++){
	$j = mt_rand(0,15);
	$str .= dechex($j);
	
    }
    return($str);
}

function setIPAddr(&$DATA){
    $iplist   = split("\.",$DATA['ipaddr']);
    $DATA['iprev']    = sprintf("%d.%d.%d.%d.in-addr.arpa",$iplist[3],$iplist[2],$iplist[1],$iplist[0]);

    switch($iplist[2]){
    case 48:
	$IPv6Prefix = "2001:2f8:1c:d048";
	$IPv6Rev    = "0.0.0.0.0.0.0.0.0.0.0.0.0.8.4.d.c.1.0.0.8.f.2.0.1.0.0.2.ip6.arpa";
        $DATA['ipv6addr'] = sprintf("%s::%02x%02x:%02x%02x",
                                    $IPv6Prefix,
                                    $iplist[0],$iplist[1],$iplist[2],$iplist[3]);
        $DATA['ipv6rev']  = sprintf("%x.%x.%x.%x.%s",
                                    $iplist[3]%16,$iplist[3]/16,$iplist[2]%16,$iplist[2]/16,
                                    $IPv6Rev);
        $DATA['dom_r6']  = "rev6_48";
        break;
    
    case 49:
	$IPv6Prefix = "2001:2f8:1c:d049";
	$IPv6Rev    = "d.0.5.8.0.0.0.0.0.0.0.0.0.9.4.d.c.1.0.0.8.f.2.0.1.0.0.2.ip6.arpa";
        $DATA['ipv6addr'] = sprintf("%s::%02x%02x:%02x%02x",
                                    $IPv6Prefix,
                                    $iplist[0],$iplist[1],$iplist[2],$iplist[3]);
        $DATA['ipv6rev']  = sprintf("%x.%x.%x.%x.%s",
                                    $iplist[3]%16,$iplist[3]/16,$iplist[2]%16,$iplist[2]/16,
                                    $IPv6Rev);
        $DATA['dom_r6']  = "rev6_49";
        break;
    case 50:
        if(0 <= $iplist[3] && $iplist[3] <= 63){
            $IPv6Prefix = "2001:2f8:1c:d04a";
            $IPv6Rev    = "0.0.0.0.0.0.0.0.0.0.0.0.0.a.4.d.c.1.0.0.8.f.2.0.1.0.0.2.ip6.arpa";
            $DATA['ipv6addr'] = sprintf("%s::%x",$IPv6Prefix,$iplist[3]);
            $DATA['ipv6rev']  = sprintf("%x.%x.%x.%x.%s",$iplist[3]%16,$iplist[3]/16,0,0,$IPv6Rev);
            $DATA['dom_r6']  = "rev6_4a";
            
        } else if( 64 <= $iplist[3] && $iplist[3] <= 127){
            $IPv6Prefix = "2001:2f8:1c:d04b";
            $IPv6Rev    = "0.0.0.0.0.0.0.0.0.0.0.0.0.b.4.d.c.1.0.0.8.f.2.0.1.0.0.2.ip6.arpa";
            $DATA['ipv6addr'] = sprintf("%s::%x",$IPv6Prefix,$iplist[3]-64);
            $DATA['ipv6rev']  = sprintf("%x.%x.%x.%x.%s",
                                        ($iplist[3]-64)%16,($iplist[3]-64)/16,0,0,$IPv6Rev);
            $DATA['dom_r6']  = "rev6_4b";
        } else if(128 <= $iplist[3] && $iplist[3] <= 191){
            $IPv6Prefix = "2001:2f8:1c:d04c";
            $IPv6Rev    = "0.0.0.0.0.0.0.0.0.0.0.0.0.c.4.d.c.1.0.0.8.f.2.0.1.0.0.2.ip6.arpa";
            $DATA['ipv6addr'] = sprintf("%s::%x",$IPv6Prefix,$iplist[3]-128);
            $DATA['ipv6rev']  = sprintf("%x.%x.%x.%x.%s",
                                        ($iplist[3]-128)%16,($iplist[3]-128)/16,0,0,$IPv6Rev);
            $DATA['dom_r6']  = "rev6_4c";
        }
        break;
    case 51:
	$IPv6Prefix = "2001:2f8:1c:d04d";
	$IPv6Rev    = "0.0.0.0.0.0.0.0.0.0.0.0.0.d.4.d.c.1.0.0.8.f.2.0.1.0.0.2.ip6.arpa";
        $DATA['ipv6addr'] = sprintf("%s::%x",$IPv6Prefix,$iplist[3]);
        $DATA['ipv6rev']  = sprintf("%x.%x.%x.%x.%s",$iplist[3]%16,$iplist[3]/16,0,0,$IPv6Rev);
        $DATA['dom_r6']  = "rev6_4d";
        break;        
    case 52:
        if( 0 <= $iplist[3] && $iplist[3] <= 127){
            $IPv6Prefix = "2001:2f8:1c:d04e";
            $IPv6Rev    = "0.0.0.0.0.0.0.0.0.0.0.0.0.e.4.d.c.1.0.0.8.f.2.0.1.0.0.2.ip6.arpa";
            $DATA['ipv6addr'] = sprintf("%s::%x",$IPv6Prefix,$iplist[3]);
            $DATA['ipv6rev']  = sprintf("%x.%x.%x.%x.%s",$iplist[3]%16,$iplist[3]/16,0,0,$IPv6Rev);
            $DATA['dom_r6']  = "rev6_4e";
        } else if(128 <= $iplist[3] && $iplist[3] <= 191){
            $IPv6Prefix = "2001:2f8:1c:d04f";
            $IPv6Rev    = "0.0.0.0.0.0.0.0.0.0.0.0.0.d.4.d.c.1.0.0.8.f.2.0.1.0.0.2.ip6.arpa";
            $DATA['ipv6addr'] = sprintf("%s::%x",$IPv6Prefix,$iplist[3]-128);
            $DATA['ipv6rev']  = sprintf("%x.%x.%x.%x.%s",
                                        ($iplist[3]-128)%16,($iplist[3]-128)/16,0,0,$IPv6Rev);
            $DATA['dom_r6']  = "rev6_4f";
        }
        break;
    default:
	$IPv6Prefix = "2001:2f8:1c:d050";
	$IPv6Rev    = "d.0.5.8.0.0.0.0.0.0.0.0.0.5.0.d.c.1.0.0.8.f.2.0.1.0.0.2.ip6.arpa";
        $DATA['ipv6addr'] = sprintf("%s::%02x%02x:%02x%02x",
                                    $IPv6Prefix,
                                    $iplist[0],$iplist[1],$iplist[2],$iplist[3]);
        $DATA['ipv6rev']  = sprintf("%x.%x.%x.%x.%s",
                                    $iplist[3]%16,$iplist[3]/16,$iplist[2]%16,$iplist[2]/16,
                                    $IPv6Rev);        
        $DATA['dom_r6']  = "rev6_50";
        break;
    }
    
    return($DATA);
}

function strMailLink($mail,$len = 15){
    $str = sprintf("<a href=\"mailto:%s\">%s%s</a>",
		   $mail,mb_substr($mail,0,$len),
		   (strlen($mail) > $len) ? "...": "");
    return($str);
}

function copyArray(&$dst,&$src){
    foreach($src as $key => $value){
	if(!ctype_digit((string)$key)){
	    $dst[$key] = $value;
	}
    }
}

function readData($DNS,&$DATA,$LDAP_DATA){
    $_DATA = array();
    if(isset($_POST['id'])){
	$DATA = $DNS->getDataByID($_POST['id']);
	$replace = ($DATA['domain'] == "ie") ? ".ie.u-ryukyu.ac.jp" : ".".$DATA['domain'].".ie.u-ryukyu.ac.jp";

	switch(isset($_GET['type']) ? $_GET['type'] : ""){
	case "A":
	    $DATA['host_name'] = str_replace($replace,"",$DATA['name']);
	    $DATA['ipaddr'] = $DATA['rdata'];
	    if($DATA['ipv6'] == 't'){
		setIPAddr($DATA);
	    }
	    $DATA['name_old'] = $DATA['name'];
	    break;
	case "CNAME":
	    $DATA['host_name'] = str_replace($replace,"",$DATA['name']);
	    $DATA['alias']     = str_replace($replace,"",$DATA['rdata']);
	    $DATA['name_old'] = $DATA['name'];
	    break;
	case "MX":
	    list($DATA['preference'],$DATA['mailserver']) = split("\ ",$DATA['rdata']);
	    break;
	}
    }
    /* $_POST['cmd'] が存在する場合は新規登録ではないので POST or SESSION からコピー */
    
    if(isset($_POST['cmd'])){
	foreach($_POST as $key => $value){
	    if(!isset($DATA[$key])){
		$DATA[$key] = $value;
	    }
	}
	foreach($_SESSION as $key => $value){
	    if(!isset($DATA[$key])){
		$DATA[$key] = $value;
	    }
	}
    }

    if(!empty($DATA['macaddr_wired'])){
	$DATA['macaddr_wired'] = strtolower($DATA['macaddr_wired']);
    }
    
    if(!empty($DATA['macaddr_wireless'])){
	$DATA['macaddr_wireless'] = strtolower($DATA['macaddr_wireless']);
    }
    
    if(!isset($DATA['sid'])){
	$DATA['sid']       = $LDAP_DATA['uid'][0];
    }
    
    if(!isset($DATA['user_name'])){
	$DATA['user_name'] = $LDAP_DATA['cn'][0];
    }
    
    if(!isset($DATA['mail']) && isset($LDAP_DATA['mail'])){
	$DATA['mail']      = $LDAP_DATA['mail'][0];
    }
        
    switch(isset($_GET['type']) ? $_GET['type'] : ""){
    case "A":
	if(isset($DATA['host_name'])){
	    if($DATA['domain'] == "ie"){
		$DATA['name']  = sprintf("%s.ie.u-ryukyu.ac.jp",$DATA['host_name']);
	    } else {
		$DATA['name']  = sprintf("%s.%s.ie.u-ryukyu.ac.jp",$DATA['host_name'],$DATA['domain']);
	    }
	}
	break;
    case "CNAME":
	if(isset($DATA['host_name'])){
	    $DATA['name']  = $DATA['domain'] == "ie"  ?
		sprintf("%s.ie.u-ryukyu.ac.jp",$DATA['host_name']):
		sprintf("%s.%s.ie.u-ryukyu.ac.jp",$DATA['host_name'],$DATA['domain']);
	}
	if(isset($DATA['alias'])){
	    $DATA['rdata'] = $DATA['domain'] == "ie"  ?
		sprintf("%s.ie.u-ryukyu.ac.jp",$DATA['alias']):
		sprintf("%s.%s.ie.u-ryukyu.ac.jp",$DATA['alias'],$DATA['domain']);
	}
	break;
    case "MX":
	if(isset($DATA['mailserver'])){
	    $DATA['rdata'] = sprintf("%s %s",$DATA['preference'],$DATA['mailserver']);
	}
	break;
    }
    
    if(isset($_POST['cmd']) && $_POST['cmd'] == "confirm"){
	$_SESSION['cmdExec'] = true;
    }
    if(isset($_POST['cmd']) && $_POST['cmd'] == "regist"){
	$DATA['hash'] = genHash(40);
    } 
    
}

function cmdExec($DNS,$DHCP,&$DATA){
    if(isset($_GET['type'])){
	$func = "cmdExec".$_GET['type'];
	if(function_exists($func)){
	    call_user_func_array($func,array($DNS,$DHCP,&$DATA));
	}
    } else {
	if(isset($_POST['cmd']) && $_POST['cmd']  == "alert" && isset($_POST['id'])){
	    $DNS->updateAlert($_POST['id']);
	    $_SESSION = array();
	}
    }
}

function cmdExecA($DNS,$DHCP,&$DATA){
    global $DHCP_USE;
    if((isset($_GET['cmd']) && $_GET['cmd'] == "new") && 
       (isset($_POST['cmd']) && $_POST['cmd']  == "regist") &&
       (isset($_SESSION['cmdExec']) && $_SESSION['cmdExec'])){
	$DATA['ipaddr'] = $DNS->getFreeIPAddr();
	$iplist   = split("\.",$DATA['ipaddr']);
	setIPAddr($DATA);
	
	$DNS->insertARecord($DATA);
	if( 56  <= $iplist[2] && $iplist[2] <=63){
	    if($DHCP_USE){
		$DHCP->insert(str_replace(".ie.u-ryukyu.ac.jp","",$DATA['name']),
			      sprintf("%d.%d.%d.%d",$iplist[0],$iplist[1],$iplist[2],$iplist[3]),
			      $DATA['macaddr_wired']);
	    }
	}
	
	sendMailARecode($DATA);
    } else if((isset($_GET['cmd']) && $_GET['cmd'] == "edit") && 
	      (isset($_POST['cmd']) && $_POST['cmd']  == "regist") &&
	      (isset($_SESSION['cmdExec']) && $_SESSION['cmdExec'])){
	$iplist   = split("\.",$DATA['ipaddr']);
	setIPAddr($DATA);
	$DNS->updateARecord($DATA);

	if( 56  <= $iplist[2] && $iplist[2] <=63){
	    if($DHCP_USE){
		$DHCP->delete(str_replace(".ie.u-ryukyu.ac.jp","",$DATA['name_old']));
		$DHCP->insert(str_replace(".ie.u-ryukyu.ac.jp","",$DATA['name']),
			      sprintf("%d.%d.%d.%d",$iplist[0],$iplist[1],$iplist[2],$iplist[3]),
			      $DATA['macaddr_wired']);
	    }
	}
	sendMailARecode($DATA);
    } else if((isset($_GET['cmd']) && $_GET['cmd'] == "delete") && 
	      (isset($_POST['cmd']) && $_POST['cmd']  == "delete") &&
	      (isset($_SESSION['cmdExec']) && $_SESSION['cmdExec'])){
	$iplist   = split("\.",$DATA['ipaddr']);
	$DNS->deleteARecord($DATA);
	if( 56  <= $iplist[2] && $iplist[2] <=63){
	    if($DHCP_USE){
		$DHCP->delete(str_replace(".ie.u-ryukyu.ac.jp","",$DATA['name_old']));
	    }
	}
	sendMailARecode($DATA,"delete");
    } else if((isset($_GET['cmd']) && $_GET['cmd'] == "list") && 
	      (isset($_POST['cmd']) && $_POST['cmd']  == "alert" && isset($_POST['id']))){
	$DNS->updateAlert($_POST['id']);
    }
}    

function cmdExecCNAME($DNS,$DHCP,&$DATA){
    if((isset($_GET['cmd']) && $_GET['cmd'] == "new") && 
       (isset($_POST['cmd']) && $_POST['cmd']  == "regist") &&
       (isset($_SESSION['cmdExec']) && $_SESSION['cmdExec'])){
	$DNS->insertCNAMERecord($DATA);
	sendMailCNAMERecord($DATA);
    } else if((isset($_GET['cmd']) && $_GET['cmd'] == "edit") && 
	      (isset($_POST['cmd']) && $_POST['cmd']  == "regist") &&
	      (isset($_SESSION['cmdExec']) && $_SESSION['cmdExec'])){
	$DNS->updateCNAMERecord($DATA);
	sendMailCNAMERecord($DATA);
    } else if((isset($_GET['cmd']) && $_GET['cmd'] == "delete") && 
	      (isset($_POST['cmd']) && $_POST['cmd']  == "delete") && 
	      (isset($_SESSION['cmdExec']) && $_SESSION['cmdExec'])){
	$DNS->deleteCNAMERecord($DATA);
	sendMailCNAMERecord($DATA,"delete");
    }
}

function cmdExecMX($DNS,$DHCP,&$DATA){
    if((isset($_POST['cmd']) && $_POST['cmd'] == "regist") && 
       (isset($_SESSION['cmdExec']) && $_SESSION['cmdExec'])){
	$DNS->updateMXRecord($DATA);
	sendMailMXRecord($DATA);
    }
}


function cmdExecSOA($DNS,$DHCP,&$DATA){
    if(isset($_POST['cmd']) && $_POST['cmd'] == "update" && isset($_POST['domain'])){
	$DNS->updateSerial($_POST['domain']);
	$_SESSION = array();
    }
}

function ABody($DNS,$DATA,$ERROR){
    global $DOMAIN_LIST;
    global $NUM_RECORD;

    if(isset($_GET['cmd']) && $_GET['cmd'] == "list"){
	if(isset($_GET['domain'])){
	    $page = isset($_GET['page']) ? $_GET['page'] : 1;
	    $cnt  = $DNS->countbyDomain($_GET['domain']);
	    $str = sprintf("所属別 登録一覧 %s(%s) ページ %s<br />\n",$DOMAIN_LIST[$_GET['domain']]['name'],strtoupper($_GET['domain']),$page);
	    for($i=1;$i <= ceil($cnt / $NUM_RECORD);$i++){
		$str .= sprintf("[<a href=\"%s?type=A&cmd=list&domain=%s&page=%d\">%s</a>]\n",$_SERVER['SCRIPT_NAME'],$_GET['domain'],$i,$i);
	    }
	    $str .= sprintf("[<a href=\"%s?type=A&cmd=list&domain=%s&page=ALL\">ALL</a>]\n",$_SERVER['SCRIPT_NAME'],$_GET['domain']);
	    h3($str);
	    printListbyDomain($DNS,$_GET['domain'],$page,$DOMAIN_LIST[$_GET['domain']]['isEdit']);
	    return(true);
	    
	} else if(isset($_GET['ipaddr'])){
	    $page = isset($_GET['page']) ? $_GET['page'] : 1;
	    $cnt  = $DNS->countbyIPAddr($_GET['ipaddr']);
	    
	    $str = sprintf("IPアドレス別 登録一覧 133.13.%d.0 ページ %s<br />\n",$_GET['ipaddr'],$page);
	    for($i=1;$i <= ceil($cnt / $NUM_RECORD);$i++){
		$str .= sprintf("[<a href=\"%s?type=A&cmd=list&ipaddr=%d&page=%d\">%s</a>]\n",
				$_SERVER['SCRIPT_NAME'],$_GET['ipaddr'],$i,$i);
	    }
	    $str .= sprintf("[<a href=\"%s?type=A&cmd=list&ipaddr=%s&page=ALL\">ALL</a>]\n",
			    $_SERVER['SCRIPT_NAME'],$_GET['ipaddr']);
	    h3($str);
	    printListbyIPAddr($DNS,$_GET['ipaddr'],$page,$DOMAIN_LIST['isEdit']);
	    return(true);

	} else {
	    h3("登録データ一覧");
	    printListbyUser($DNS,$_SERVER['PHP_AUTH_USER'],true);
	    return(true);
	}
    } else if(isset($_GET['cmd']) && ($_GET['cmd'] == "new" || $_GET['cmd'] == "edit")){
	if(isset($_POST['cmd']) && $_POST['cmd'] == "confirm" && empty($ERROR)){
	    h3("ユーザ・システム情報の確認");
	    p("以下の内容で登録します。入力内容を確認してください。");
	    printARecordDATA($DATA,"edit");
	    return(true);
	} else if(isset($_POST['cmd']) && $_POST['cmd'] == "regist"){
	    if(isset($_SESSION['cmdExec']) && $_SESSION['cmdExec']){
		h3("ユーザ・システム情報の登録");
		p("以下の内容で登録しました。\n".
		  "確認のメールを送信しました。<br />\n".
		  "確認のメールが届いていない場合は<a href=\"mailto:host-regist@ie.u-ryukyu.ac.jp\">host-regist at ie.u-ryukyu.ac.jp</a>に連絡してください。<br />\n".
		  "<a href=\"./index.php\">トップページへ戻る</a>\n");
		printARecordDATA($DATA);
		$_SESSION = array();
		return(true);
	    } else {
		return(false);
	    }
	} else {
	    h3("ユーザ・システム情報の編集");
	    printARecordEditForm($DATA,$ERROR,$DOMAIN_LIST['isEdit']);
	    
	    if(!empty($ERROR)){
		printErrMSG($DATA,$ERROR);
	    }

	    return(true);
	}
    } else if(isset($_GET['cmd']) && $_GET['cmd'] == "delete"){
	if(isset($_POST['cmd']) && $_POST['cmd'] == "delete"){
	    if(isset($_SESSION['cmdExec']) && $_SESSION['cmdExec']){
		h3("ユーザ・システム情報の削除");
		p("以下の内容を削除しました。<br />\n".
		  "<a href=\"./index.php\">トップページへ戻る</a>\n");
		printARecordDATA($DATA);
		$_SESSION = array();
		return(true);
	    } else {
		return(false);
	    }
	} else {
	    h3("ユーザ・システム情報の確認");
	    p("以下の内容を削除します。\n".
	      "間違えなければ削除を選択してください。\n");
	    printARecordDATA($DATA,"delete");
	    
	    return(true);
	}
    }
    return(false);
}

function MXBody($DNS,$DATA,$ERROR){
    global $DOMAIN_LIST;
    if(isset($_GET['cmd']) && $_GET['cmd'] == "list"){
	h3("登録一覧 MXレコード");
	printMXRecordList($DNS,true);
	return(true);
    } else if(isset($_GET['cmd']) && ($_GET['cmd'] == "new" || $_GET['cmd'] == "edit")){
	if(isset($_POST['cmd']) && $_POST['cmd'] == "confirm"  && empty($ERROR)){
	    h3("MXレコード登録情報の確認");
	    p("以下の内容で登録します。入力内容を確認してください。");
	    printMXRecordDATA($DATA,"edit");
	    return(true);
	} else if(isset($_POST['cmd'])&& $_POST['cmd'] == "regist" && empty($ERROR)){
	    if(isset($_SESSION['cmdExec']) && $_SESSION['cmdExec']){
		h3("MXレコードの登録");
		p("以下の内容で登録しました。\n".
		  "確認のメールを送信しました。<br />\n".
		  "確認のメールが届いていない場合は<a href=\"mailto:host-regist@ie.u-ryukyu.ac.jp\">host-regist at ie.u-ryukyu.ac.jp</a>に連絡してください。<br />\n".
		  "<a href=\"./index.php\">トップページへ戻る</a>\n");
		printMXRecordDATA($DATA);
		$_SESSION = array();
		return(true);
	    } else {
		return(false);
	    }
	} else {	
	    h3("MXレコード登録情報の編集");    
	    printMXRecordEditForm($DATA,$ERROR,true);
	    if(!empty($ERROR)){
		printErrMSG($DATA,$ERROR);
	    }
	    return(true);
	}
    }
    return(false);
}

function CNAMEBody($DNS,$DATA,$ERROR){
    global $DOMAIN_LIST;
    if(isset($_GET['cmd']) && $_GET['cmd'] == "list"){
	h3(" CNAMEレコード 登録一覧");
	printCNAMERecordList($DNS,true);
	return(true);
    } else if(isset($_GET['cmd']) && ($_GET['cmd'] == "new" || $_GET['cmd'] == "edit")){
	if(isset($_POST['cmd']) && $_POST['cmd'] == "confirm" && empty($ERROR)){
	    h3("CNAMEレコードの確認");
	    p("以下の内容で登録します。入力内容を確認してください。");
	    printCNAMERecordDATA($DATA,"edit");
	    return(true);
	} else if(isset($_POST['cmd'])&& $_POST['cmd'] == "regist"){
	    if(isset($_SESSION['cmdExec']) && $_SESSION['cmdExec']){
		h3("CNAMEレコードの登録");
		p("以下の内容で登録しました。\n".
		  "確認のメールを送信しました。<br />\n".
		  "確認のメールが届いていない場合は<a href=\"mailto:host-regist@ie.u-ryukyu.ac.jp\">host-regist @ ie.u-ryukyu.ac.jp</a>に連絡してください。<br />\n".
		  "<a href=\"./index.php\">トップページへ戻る</a>\n");
		$_SESSION = array();
		printCNAMERecordDATA($DATA);
		return(true);
	    } else {
		return(false);
	    }
	}else {
	    h3("CNAMEレコード 登録情報情報の編集");
	    if(!$DOMAIN_LIST['isEdit']){
		p("管理者権限を持つユーザしか実行できません。<br />".
		  "<a href=\"./index.php\">トップページへ戻る</a>\n");
	    } else {
		printCNAMERecordEditForm($DATA,$ERROR);
		if(!empty($ERROR)){
		    printErrMSG($DATA,$ERROR);
		}
	    }
	    return(true);
	}
	
    } else if(isset($_GET['cmd']) && $_GET['cmd'] == "delete"){
	if(isset($_POST['cmd']) && $_POST['cmd'] == "delete"){
	    if(isset($_SESSION['cmdExec']) && $_SESSION['cmdExec']){
		h3("CNAME レコード登録情報の削除");
		p("以下の内容を削除しました。".
		  "<a href=\"./index.php\">トップページへ戻る</a>");
		printCNAMERecordDATA($DATA);
		$_SESSION = array();
		return(true);
	    } else {
		return(false);
	    }
	} else {
	    h3("CNAME レコード 登録情報の確認");
	    p("以下の内容を削除します。".
	      "間違えなければ削除を選択してください。");
	    printCNAMERecordDATA($DATA,"delete");
	    return(true);
	}
    }
    return(false);
}

function SOABody($DNS,$DATA,$ERROR){
    global $DOMAIN_LIST;
    if(isset($_GET['cmd']) && $_GET['cmd'] == "list"){
	h3("登録一覧 SOAレコード");
	printSOARecordList($DNS,true);
	return(true);
    }
    return(false);
}