<?

function sendMailARecode($DATA,$mode = NULL){
    global $SEND_FROM;
    global $DOMAIN_LIST;
    global $ADMIN_MAIL;
    global $SEND_FROM;
    
    $iplist   = split("\.",$DATA['ipaddr']);
    $DATA['ipaddr'] = preg_replace("/\.0{1,2}/",".",$DATA['ipaddr']);
    
    $from = 'From: ' . $SEND_FROM;  

    
    if($mode == "delete"){
	$subject = "IPアドレス 削除 from " . $_SERVER['REMOTE_ADDR'] ;
	$msg = sprintf("以下の内容を削除しました。\n");
    } else {
	$subject = "IPアドレス 申請 from " . $_SERVER['REMOTE_ADDR'] ;
	$msg = sprintf("以下の内容で申請を受け付けました。\n");
    }	
    $msg .= sprintf("  登録アカウント      : %s\n".
		    "  使用者名            : %s\n". 
		    "  使用者メールアドレス: %s\n",
		    $DATA['sid'],$DATA['user_name'],$DATA['mail']);
    
    $msg .= sprintf("  ホスト名            : %s\n".
		    "  IPアドレス          : %s\n",
		    $DATA['name'],$DATA['ipaddr']);
    if(isset($DATA['ipv6addr']) && $DATA['ipv6'] == 't'){
	$msg .= sprintf("  IPv6アドレス        : %s\n",$DATA['ipv6addr']);
    }
    
    $msg .= 
	sprintf("  所属(ドメイン)      : %s(%s)\n".
		"  MACアドレス（有線） : %s\n".
		"  MACアドレス（無線） : %s\n".
		"  IPv6 固定割り当て   : %s\n".
		"  MXレコード          : %s\n".
		"  備考欄              : %s\n",
		$DOMAIN_LIST[$DATA['domain']]['name'],$DATA['domain'],
		$DATA['macaddr_wired'],$DATA['macaddr_wireless'],
		$DATA['ipv6'] == 't' ? "有効" : "無効",
		$DATA['mx'] == 't' ? "有効" : "無効",
		$DATA['description']);
    
    if(56 <= $iplist[2] && $iplist[2] <= 63 && !($mode == "delete")){
	$msg .=  "固定アドレスとして利用する場合は以下の情報を利用ください。\n";
	$msg .= 
	    "  Netmask             : 255.255.248.0\n".
	    "  GateWay             : 133.13.63.254\n".
	    "  DNSサーバー         : 133.13.48.3\n".
	    "                      : 133.13.48.2\n";
    }
    mb_language("Ja");
    mb_internal_encoding("UTF-8");
    
    $subject = mb_convert_encoding($subject,"UTF-8");
    $msg     = mb_convert_encoding($msg    ,"UTF-8");
    
    $ret = mb_send_mail($DATA['mail'],$subject,$msg,$from);
    /*
    foreach($DOMAIN_LIST[$DATA['domain']]['mail'] as $value){
	$ret = mb_send_mail($value,$subject,$msg,$from);
    }
    */
    $ret = mb_send_mail($ADMIN_MAIL,$subject,$msg,$from);
}

function sendMailCNAMERecord($DATA,$mode = NULL){
    global $SEND_FROM;
    global $DOMAIN_LIST;
    global $ADMIN_MAIL;
    global $SEND_FROM;
    
    $from = 'From: ' . $SEND_FROM;  
        
    if($mode == "delete"){
	$subject = "CNAMEレコード 削除 from " . $_SERVER['REMOTE_ADDR'] ;
	$msg = sprintf("以下の内容を削除しました。\n");
    } else {
	$subject = "CNAMEレコード 申請 from " . $_SERVER['REMOTE_ADDR'] ;
	$msg = sprintf("以下の内容でCNAME レコードの申請を受け付けました。\n");
    }  
    
    $msg .= " 別名ホスト名   : " . $DATA['name']  ."\n";
    $msg .= " 参照先ホスト名 : " . $DATA['rdata'] ."\n";

    mb_language("Ja");
    mb_internal_encoding("UTF-8");
    
    $subject = mb_convert_encoding($subject,"UTF-8");
    $msg     = mb_convert_encoding($msg    ,"UTF-8");
    foreach($DOMAIN_LIST[$DATA['domain']]['mail'] as $value){
	$ret = mb_send_mail($value,$subject,$msg,$from);
    }
    $ret = mb_send_mail($ADMIN_MAIL,$subject,$msg,$from);
}

function sendMailMXRecord($DATA,$mode = NULL){
    global $SEND_FROM;
    global $DOMAIN_LIST;
    global $ADMIN_MAIL;
    global $SEND_FROM;
    
    $from = 'From: ' . $SEND_FROM;
    
    $subject = "MXレコード 申請 from " . $_SERVER['REMOTE_ADDR'] ;
    $msg = sprintf("以下の内容でMX レコードの申請を受け付けました。\n");
    
    $msg .= "  ドメイン名     : ".$DATA['name']."\n";
    $msg .= "  転送先ホスト名 : ".$DATA['mailserver']."\n";

    mb_language("Ja");
    mb_internal_encoding("UTF-8");
    
    $subject = mb_convert_encoding($subject,"UTF-8");
    $msg     = mb_convert_encoding($msg    ,"UTF-8");
    foreach($DOMAIN_LIST[$DATA['domain']]['mail'] as $value){
	$ret = mb_send_mail($value,$subject,$msg,$from);
    }
    $ret = mb_send_mail($ADMIN_MAIL,$subject,$msg,$from);
}

?>
