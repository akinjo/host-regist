<?php
function printListbyUser($DNS,$user){
    $DNS->queryListbyUser($user);
    printList($DNS,true);
}

function printListbyDomain($DNS,$domain = "st",$page = 0,$isEdit = false){
    $DNS->queryListbyDomain($domain,"rdata",$page);
    printList($DNS,false);
}

function printListbyIPAddr($DNS,$ipaddr = 48,$page = 0){
    $DNS->queryListbyIPAddr($ipaddr,"rdata",$page);
    printList($DNS,false);
}

function strEditButtom($id,$type){
    $str  = sprintf("    <form method=\"POST\" action=\"%s?type=%s&cmd=edit\" />\n",$_SERVER['SCRIPT_NAME'],$type);
    $str .= sprintf("     <input type=\"submit\" value=\"編集\">\n");
    $str .= sprintf("     <input type=\"hidden\" name=\"id\"  value=\"%s\" />\n",$id);
    $str .= sprintf("    </form>\n");
    return($str);
}

function strDeleteButtom($id,$type){
    $str  = sprintf("    <form method=\"POST\" action=\"%s?type=%s&cmd=delete\" />\n",$_SERVER['SCRIPT_NAME'],$type);
    $str .= sprintf("     <input type=\"submit\" value=\"削除\">\n");
    $str .= sprintf("     <input type=\"hidden\" name=\"id\"  value=\"%s\" />\n",$id);
    $str .= sprintf("     <input type=\"hidden\" name=\"cmd\" value=\"confirm\" />\n");
    $str .= sprintf("    </form>\n");
    return($str);
}

function strAlertButtom($id){
    $str  = sprintf("    <form method=\"POST\" action=\"%s\" />\n",$_SERVER['REQUEST_URI']);
    $str .= sprintf("     <input type=\"submit\" value=\"警告\">\n");
    $str .= sprintf("     <input type=\"hidden\" name=\"id\"  value=\"%s\" />\n",$id);
    $str .= sprintf("     <input type=\"hidden\" name=\"cmd\"  value=\"alert\" />\n");
    $str .= sprintf("    </form>\n");
    return($str);
}

function strUpdateButtom($domain){
    $str  = sprintf("    <form method=\"POST\" action=\"%s\" />\n",$_SERVER['REQUEST_URI']);
    $str .= sprintf("     <input type=\"submit\" value=\"更新\">\n");
    $str .= sprintf("     <input type=\"hidden\" name=\"domain\"  value=\"%s\" />\n",$domain);
    $str .= sprintf("     <input type=\"hidden\" name=\"cmd\"  value=\"update\" />\n");
    $str .= sprintf("    </form>\n");
    return($str);
}

function printList($DNS,$edit = false){
    global $IPv6Prefix;
    global $DOMAIN_LIST;
    
    $i = 0;

    $hEdit = ($DNS->countbySIDMatch($_SERVER['PHP_AUTH_USER']) > 0) ? true : false;
    
    print "<table id=\"list\">";

    while($DATA = $DNS->get()){
	$DATA['ipaddr'] = preg_replace("/\.0{1,2}/",".",$DATA['rdata']);
	setIPAddr($DATA);

	if($i % 10 == 0){
	    print " <tr>\n";
	    print "  <th>ホスト名</th>\n";
	    print "  <th>IPアドレス</th>\n";
	    print "  <th>登録<br />アカウント</th>\n";
	    print "  <th>使用者名<br />メールアドレス</th>\n";
	    print "  <th>所属<br />(ドメイン)</th>\n";
	    print "  <th>MACアドレス<br />（有線,無線）</th>\n";
	    print "  <th>IPv6<br />MX</th>\n";
	    print "  <th>アクセス時刻<br />修正時刻</th>\n";
	    print "  <th>説明</th>\n";
	    
	    if($edit || $DOMAIN_LIST['isEdit'] || $hEdit){
		print "  <th>編集 </th>\n";
		print "  <th>削除 </th>\n";
		if($DOMAIN_LIST['isEdit']){
		    print "  <th>警告 </th>\n";
		}
	    }
	    print " </tr>\n";
	}
	$i++;
	printf(" <tr %s>\n",
	       ((isset($DATA['alert']) && $DATA['alert'] == "t" ) ? "id=\"alert\"" : ""));
	printf("  <td style=\"white-space: normal;\" >%s</td>\n",
	       !empty($DATA['name']) ? $DATA['name']: "&nbsp;");
	printf("  <td>%s<br />%s</td>\n",
	       (!empty($DATA['ipaddr']) ? $DATA['ipaddr']: "&nbsp;") ,
	       ($DATA['ipv6'] == "t" ? $DATA['ipv6addr'] : "-" ));
	printf("  <td>%s</td>\n",
	       !empty($DATA['sid']) ? $DATA['sid']: "&nbsp;");
	printf("  <td>%s<br />%s</td>\n",
	       !empty($DATA['user_name']) ? $DATA['user_name']: "&nbsp;",
	       !empty($DATA['mail']) ? strMailLink($DATA['mail']): "&nbsp;");
	printf("  <td>%s<br />(%s)</td>\n",
	       !empty($DATA['domain']) ? $DOMAIN_LIST[$DATA['domain']]['name'] : "&nbsp;",
	       !empty($DATA['domain']) ? strtoupper($DATA['domain']) : "&nbsp;");
	printf("  <td style=\"text-align:center;  font-family: monospace;\">%s<br />%s</td>\n",
	       !empty($DATA['macaddr_wired'])    ? $DATA['macaddr_wired']     : "-",
	       !empty($DATA['macaddr_wireless']) ? $DATA['macaddr_wireless']  : "-");
	printf("  <td>%s<br />%s</td>\n",
	       (!empty($DATA['ipv6']) && $DATA['ipv6'] == "t") ? "有効" : "無効",
	       (!empty($DATA['mx'])   && $DATA['mx'] == "t") ? "有効" : "無効");
	printf("  <td>%s<br />%s</td>\n",
	       !empty($DATA['atime']) ? substr($DATA['atime'],0,10) : "&nbsp;",
	       !empty($DATA['mtime']) ? substr($DATA['mtime'],0,10) : "&nbsp;");
	printf("  <td style=\"white-space: normal;\" >%s</td>\n",
	       !empty($DATA['description']) ? $DATA['description'] : "&nbsp;");
	
	if($edit || $DOMAIN_LIST['isEdit'] || $hEdit){
	    printf("  <td>\n%s</td>\n",
		   $edit || ($DOMAIN_LIST[$DATA['domain']]['isEdit'] || $DATA['sid'] == $_SERVER['PHP_AUTH_USER']) ?
		   strEditButtom($DATA['id'],"A") : "&nbsp;");
	    printf("  <td>\n%s</td>\n",
		   (isset($DATA['delete']) && $DATA['delete'] == 't' && ($edit || $DOMAIN_LIST[$DATA['domain']]['isEdit'])  || $DATA['sid'] == $_SERVER['PHP_AUTH_USER'])
		   ? strDeleteButtom($DATA['id'],"A") : "&nbsp;");
	    if($DOMAIN_LIST['isEdit']){
		printf("  <td>\n%s</td>\n",
		       $DOMAIN_LIST[$DATA['domain']]['isEdit'] ? strAlertButtom($DATA['id']) : "&nbsp;");
	    }
	    printf(" </tr>\n");
	}
    }
    printf("</table>\n");
}


function printSOARecordList($DNS){
    global $DOMAIN_LIST;
    $DNS->querySOA();
    $i = 0;
    
    print "<table id=\"list\">\n";
    while($DATA = $DNS->get()){
	list($mname,$rname,$serial,$refresh,$retry,$expire,$minimum) = split("\ ",$DATA['rdata']);

	if($i  % 10 == 0){
	    print " <tr>\n";
	    print "  <th>DOMAIN</th>\n";
	    print "  <th>MNAME</th>\n";
	    print "  <th>RENAME</th>\n";
	    print "  <th>SERIAL</th>\n";
	    print "  <th>REFRESH</th>\n";
	    print "  <th>RETRY</th>\n";
	    print "  <th>EXPIRE</th>\n";
	    print "  <th>MINUMUM</th>\n";
	    if($DOMAIN_LIST['isEdit']){
		print "  <th>更新</th>\n";
	    }
	}
	$i++;
	printf(" <tr>\n");
	printf("  <td>%s</td>\n", $DATA['name']);
	printf("  <td>%s</td>\n",$mname);
	printf("  <td>%s</td>\n",$rname);
	printf("  <td>%s</td>\n",$serial);
	printf("  <td>%s</td>\n",$refresh);
	printf("  <td>%s</td>\n",$retry);
	printf("  <td>%s</td>\n",$expire);
	printf("  <td>%s</td>\n",$minimum);
	if($DOMAIN_LIST['isEdit']){
	    printf("  <td>\n%s</td>\n",strUpdateButtom($DATA['domain']));
	}
	printf(" </tr>\n");
    }
    print "</table>\n";
}

function printMXRecordList($DNS,$isEdit = false){
    global $DOMAIN_LIST;
    $DNS->queryMX();
    $i = 0;
    print "<table id=\"list\">\n";
    
    while($DATA = $DNS->get()){
	list($DATA['preference'],$DATA['host_name']) = split("\ ",$DATA['rdata']);
	if($i  % 10  == 0 ){
	    print " <tr>\n";
	    print "  <th>ドメイン</th>\n";
	    print "  <th>プレファレンス値</th>\n";
	    print "  <th>転送先ホスト名</th>\n";
	    print "  <th>ドメイン</th>\n";
	    if($DOMAIN_LIST['isEdit']){
		print "  <th>編集</th>\n";
	    }
	    print " </tr>\n";
	}
        $i++;	
	printf(" <tr>\n");
	printf("  <td>%s</td>\n",$DATA['name']);
	printf("  <td>%s</td>\n",$DATA['preference']);
	printf("  <td>%s</td>\n",$DATA['host_name']);
	printf("  <td>%s(%s)</td>\n",
	       !empty($DATA['domain']) ? $DOMAIN_LIST[$DATA['domain']]['name'] : "&nbsp;",
	       !empty($DATA['domain']) ? strtoupper($DATA['domain']) : "&nbsp;");
	if($DOMAIN_LIST['isEdit']){
	    printf("  <td>\n%s</td>\n",
		   $DOMAIN_LIST[$DATA['domain']]['isEdit'] ? strEditButtom($DATA['id'],"MX") : "&nbsp;");
	}
	printf(" </tr>\n");
    }

    printf("</table>\n");
}

function printCNAMERecordList($DNS){
    global $DOMAIN_LIST;
    $DNS->queryCNAME("domain");
    $i=0;

    print "<table id=\"list\">\n";
    
    while($DATA = $DNS->get()){
        if($i % 10 == 0){
	    print " <tr>\n";
	    print "  <th>別名ホスト名</th>\n";
	    print "  <th>参照先ホスト名</th>\n";
	    print "  <th>所属（ドメイン）</th>\n";
	    if($DOMAIN_LIST['isEdit']){
		print "  <th>編集</th>\n";
		print "  <th>削除</th>\n";
	    }
	    print " </tr>\n";
	}
	$i++;
	print " <tr>\n";
	printf("  <td>%s</td>\n",$DATA['name']);
	printf("  <td>%s</td>\n",$DATA['rdata']);
	printf("  <td>%s(%s)</td>\n",$DOMAIN_LIST[$DATA['domain']]['name'],strtoupper($DATA['domain']));
	if($DOMAIN_LIST['isEdit']){
	    printf("<td>\n%s</td>\n",
		   $DOMAIN_LIST[$DATA['domain']]['isEdit'] ? strEditButtom($DATA['id'],"CNAME") : "&nbsp;");
	    printf("<td>\n%s</td>\n",
		   $DOMAIN_LIST[$DATA['domain']]['isEdit'] ? strDeleteButtom($DATA['id'],"CNAME") : "&nbsp;");
	}
	print " </tr>\n";
    }
    print "</table>\n";
}

function printARecordEditForm($DATA = NULL,$ERROR = NULL,$isSIDEdit = false){
    global $DOMAIN_LIST;

    print "<form method=\"POST\">\n";
    print "<table id=\"edit\">\n";
    print " <tr>\n";
    print "  <th>項目</th>\n";
    print "  <th>記入欄</th>\n";
    print " </tr>\n";
    
    printf(" <tr %s>\n",isset($ERROR['sid']) ? "id=\"alert\"" : "" );
    printf("  <td>登録者アカウント</td>\n");
    printf("   <td>%s</td>\n",
	   $isSIDEdit ?
	   sprintf("<input type=\"text\" name=\"sid\" size=\"12\" maxlength=\"12\" value=\"%s\">",(!empty($DATA['sid'])) ? $DATA['sid']: ""):
	   ((!empty($DATA['sid'])) ? $DATA['sid']: "&nbsp;"));
    printf(" </tr>\n");
    
    printf(" <tr %s>\n", isset($ERROR['user_name']) ? "id=\"alert\"" : "" );
    printf("  <td>使用者名</td>\n");
    printf("  <td><input type=\"text\" name=\"user_name\" size=\"24\" maxlength=\"24\" value=\"%s\" />\n", !empty($DATA['user_name']) ? $DATA['user_name'] : "");
    printf(" </tr>\n");
    
    printf(" <tr %s>\n", isset($ERROR['mail']) ? "id=\"alert\"" : "" );
    printf("  <td>使用者メールアドレス</td>\n");
    printf("  <td><input type=\"text\" name=\"mail\" size=\"40\" maxlength=\"40\" value=\"%s\" />\n",!empty($DATA['mail']) ? $DATA['mail'] : "");
    printf(" </tr>\n");
    
    printf(" <tr %s>\n", isset($ERROR['host_name']) ? "id=\"alert\"" : "" );
    printf("  <td>ホスト名(最大15文字)</td>\n");
    printf("  <td>\n");
    printf("   <input type=\"text\" name=\"host_name\" size=\"15\" maxlength=\"15\" value=\"%s\" />",!empty($DATA['host_name']) ? $DATA['host_name'] : "");
    printf("   <select name=\"domain\">\n");
    foreach ($DOMAIN_LIST as $key => $value) {
	if($value['select'] || (isset($DATA['domain']) && $DATA['domain'] == $key)){
	    printf("    <option value=\"%s\" %s>%s (%s)</option>\n",
		   $key,(isset($DATA['domain']) && $DATA['domain'] == $key) ? "selected" : "",$value['name'],strtoupper($key));
	}
    }
    printf("   </select>\n");
    printf("  </td>\n");
    printf(" </tr>\n");

    printf(" <tr>\n");
    printf("  <td>IPアドレス</td>\n");
    printf("  <td>%s</td>\n",!empty($DATA['ipaddr']) ? preg_replace("/\.0{1,2}/",".",$DATA['ipaddr']) : "未発行" );
    printf(" </tr>\n");
    
    if(isset($DATA['ipv6addr'])){
	printf(" <tr>\n");
	printf("  <td>IPv6アドレス</td>\n");
	printf("  <td>%s</td>\n",$DATA['ipv6addr']);
	printf(" </tr>\n");
    }
    
    printf(" <tr %s>\n", isset($ERROR['macaddr_wired']) ? "id=\"alert\"" : "" );
    printf("  <td>MACアドレス（有線）(00:00:00:00:00:00)</td>\n");
    printf("  <td><input type=\"text\" name=\"macaddr_wired\" size=\"23\" maxlength=\"17\" value=\"%s\" /></td>\n",!empty($DATA['macaddr_wired']) ? $DATA['macaddr_wired'] : "");
    printf(" </tr>\n");

    printf(" <tr %s>\n", isset($ERROR['macaddr_wireless']) ? "id=\"alert\"" : "" );
    printf("  <td>MACアドレス（無線）(00:00:00:00:00:00)</td>\n");
    printf("  <td><input type=\"text\" name=\"macaddr_wireless\" size=\"23\" maxlength=\"17\" value=\"%s\" /></td>\n",!empty($DATA['macaddr_wireless']) ? $DATA['macaddr_wireless'] : "");
    printf(" </tr>\n");

    printf(" <tr>\n");
    printf("  <td>IPv6 固定アドレス割り当て</td>\n");
    printf("  <td>\n");
    printf("   <input type=\"radio\" name=\"ipv6\" value=\"t\" %s />有効</input>\n", (isset($DATA['ipv6']) && $DATA['ipv6'] == "t" ) ? "checked" : "" );
    printf("   <input type=\"radio\" name=\"ipv6\" value=\"f\" %s />無効</input>\n", !(isset($DATA['ipv6']) && $DATA['ipv6'] == "t" ) ? "checked" : "" );
    printf("  </td>\n");
    printf(" </tr>\n");

    printf(" <tr>\n");
    printf("  <td>MXレコード</td>\n");
    printf("  <td>\n");
    printf("   <input type=\"radio\" name=\"mx\" value=\"t\" %s>有効</input>\n",(isset($DATA['mx']) && $DATA['mx'] == "t" ) ? "checked" : "" );
    printf("   <input type=\"radio\" name=\"mx\" value=\"f\" %s>無効</input>\n",!(isset($DATA['mx']) && $DATA['mx'] == "t" ) ? "checked" : "" );
    printf("  </td>\n");
    printf(" </tr>\n");

    printf(" <tr>\n");
    printf("  <td>備考欄</td>\n");
    printf("  <td><input type=\"text\" name=\"description\" size=\"40\" maxlength=\"40\" value=\"%s\" />\n",!empty($DATA['description']) ? $DATA['description'] : "");
    printf(" </tr>\n");
    
    printf(" <tr style=\"text-align:right;\">\n");
    printf("  <td colspan=\"2\">\n");
    printf("    <input type=\"submit\"  value=\"送信\" />\n");
    printf("    <input type=\"reset\"   value=\"リセット\" />\n");
    printf("    <input type=\"hidden\"  name=\"cmd\" value=\"confirm\" />\n");
    printf("  </td>\n");
    printf(" </tr>\n");
    printf("</table>\n");
    printf("<form>\n");
}


function printCNAMERecordEditForm($DATA = NULL,$ERROR = NULL){
    global $DOMAIN_LIST;
    printf("<form method=\"POST\">\n");
    printf("<table id=\"edit\">\n");
    printf(" <tr>\n");
    printf("  <th>項目</th>\n");
    printf("  <th>記入欄</th>\n");
    printf(" </tr>\n");
    
    printf(" <tr %s>\n",isset($ERROR['host_name']) ? "id=\"alert\"" : "");
    printf("  <td>別名ホスト名（[0-9a-z-]の範囲で入力）</td>\n");
    printf("  <td><input type=\"text\" name=\"host_name\" size=\"15\" maxlength=\"15\" value=\"%s\" ></td>\n",
	   !empty($DATA['host_name']) ? $DATA['host_name'] : "" );
    printf(" </tr>\n");
    
    printf(" <tr %s>\n",isset($ERROR['alias']) ? "id=\"alert\"" : "");
    printf("  <td>参照先ホスト名（[0-9a-z-]の範囲で入力）</td>\n");
    printf("  <td><input type=\"text\" name=\"alias\" size=\"15\" maxlength=\"15\" value=\"%s\"></td>\n",
	   !empty($DATA['alias']) ? $DATA['alias'] :"");
    printf(" </tr>\n");

    printf(" <tr>\n");
    printf("  <td>ドメイン</td>\n");
    printf("  <td>\n");
    printf("   <select name=\"domain\">\n");
    foreach ($DOMAIN_LIST as $key => $value) {
	if($value['select'] && $value['isEdit']){
	    printf("    <option value=\"%s\" %s>%s (%s)</option>\n",
		   $key,(isset($DATA['domain']) && $DATA['domain'] == $key) ? "selected" : "",$value['name'],strtoupper($key));
	}
    }
    printf("   </select>\n");
    printf("  </td>\n");
    printf(" </tr>\n");

    printf(" <tr>\n");
    printf("  <td>備考欄</td>\n");
    printf("  <td><input type=\"text\" name=\"description\" size=\"32\" maxlength\"32\" value=\"%s\"></td>\n",
	   !empty($DATA['description']) ? $DATA['description'] : "");
    printf(" </tr>\n");
    
    printf(" <tr style=\"text-align:right;\">\n");
    printf("  <td colspan=\"2\">\n");
    printf("   <input type=\"submit\"  value=\"送信\" />\n");
    printf("   <input type=\"reset\"   value=\"リセット\" />\n");
    printf("   <input type=\"hidden\"  name=\"cmd\" value=\"confirm\" />\n");
    printf("  </td>\n");
    printf(" </tr>\n");
    printf("</table>\n");
    printf("</form>\n");
}



function printMXRecordEditForm($DATA = NULL,$ERROR,$isSIDEdit = false){
    global $DOMAIN_LIST;
    printf("<form method=\"POST\">\n");
    printf("<table id=\"edit\">\n");
    printf(" <tr>\n");
    printf("  <th>項目</th>\n");
    printf("  <th>記入欄</th>\n");
    printf(" </tr>\n");

    printf(" <tr>\n");
    printf("  <td>ドメイン名</td>\n");
    printf("  <td>%s</td>\n",$DATA['name']);
    printf(" </tr>\n");

    printf(" </tr>\n");
    printf("  <td>プレファレンス値</td>\n");
    printf("  <td>%s</td>\n",isset($DATA['preference'])? $DATA['preference'] : "&nbsp;");
    printf(" </tr>\n");
    
    printf(" <tr %s>\n",isset($ERROR['mailserver']) ? "id=\"alert\"" : "");
    printf("  <td>転送先ホスト</td>\n");
    printf("  <td><input type=\"text\" name=\"mailserver\" size=\"40\" maxlength=\"40\" value=\"%s\"></td>\n",
	   isset($DATA['mailserver'])? $DATA['mailserver'] : "");
    printf(" </tr>\n");

    printf(" <tr style=\"text-align:right;\">\n");
    printf("  <td colspan=\"2\">\n");
    printf("   <input type=\"submit\"  value=\"送信\" />\n");
    printf("   <input type=\"reset\"   value=\"リセット\" />\n");
    printf("   <input type=\"hidden\"  name=\"cmd\" value=\"confirm\" />\n");
    printf("  </td>\n");
    printf(" </tr>\n");
    printf("</table>\n");
    printf("</form>\n");
}

function printMXRecordDATA($DATA,$mode=NULL){
    printf("<table id=\"edit\">\n");
    printf(" <tr>\n");
    printf("  <th>項目</th>\n");
    printf("  <th>記入欄</th>\n");
    printf(" </tr>\n");

    printf(" <tr>\n");
    printf("  <td>ドメイン名</td>\n");
    printf("  <td>%s</td>\n",$DATA['name']);
    printf(" </tr>\n");
    
    printf(" <tr>\n");
    printf("  <td>プレファレンス値</td>\n");
    printf("  <td>%s</td>\n",$DATA['preference']);
    printf(" </tr>\n");

    printf(" <tr>\n");
    printf("  <td>転送先ホスト名</td>\n");
    printf("  <td>%s</td>\n",$DATA['mailserver']);
    printf(" </tr>\n");
    if($mode == "edit"){
	printf(" <tr style=\"text-align:right;\">\n");	
	printf("  <td colspan=\"2\">\n");
	printf("  <form method=\"POST\" style=\"display:inline;\">\n");
	printf("   <input type=\"submit\" value=\"登録\">\n");
	printf("   <input type=\"hidden\" name=\"cmd\" value=\"regist\">\n");
	printf("  </form>\n");

	printf("  <form method=\"POST\" style=\"display:inline;\">\n");
	printf("   <input type=\"submit\" value=\"編集\">\n");
	printf("   <input type=\"hidden\" name=\"cmd\" value=\"edit\">\n");
	printf("  </form>\n");
	printf("  </td>\n");
	printf(" </tr>\n");
    }
    printf("</table>\n");
}


function printARecordDATA($DATA,$mode = NULL){
    global $DOMAIN_LIST;
    printf("<table id=\"list\">\n");

    printf(" <tr>\n");
    printf("  <th>項目</th>\n");
    printf("  <th>記入欄</th>\n");
    printf(" </tr>\n");

    printf(" <tr>\n");
    printf("  <td>登録者アカウント</td>\n");
    printf("  <td>%s</td>\n",
	   empty($DATA['sid']) ? "&nbsp;":$DATA['sid']);
    printf(" </tr>\n");

    printf(" <tr>\n");
    printf("  <td>使用者名</td>\n");
    printf("  <td>%s</td>\n",empty($DATA['user_name']) ? "&nbsp;":$DATA['user_name']);
    printf(" </tr>\n");
    
    printf(" <tr>\n");
    printf("  <td>使用者メールアドレス</td>\n");
    printf("  <td>%s</td>\n",empty($DATA['mail']) ? "&nbsp;":$DATA['mail']);
    printf(" </tr>\n");
    
    printf(" <tr>\n");
    printf("  <td>ホスト名</td>\n");
    printf("  <td>%s</td>\n",empty($DATA['name']) ? "&nbsp;":$DATA['name']);
    printf(" </tr>\n");
    
    printf(" <tr>\n");
    printf("  <td>所属</td>\n");
    printf("  <td>%s</td>\n",$DOMAIN_LIST[$DATA['domain']]['name']);
    printf(" </tr>\n");

    if(isset($DATA['ipaddr'])){
	printf(" <tr>\n");
	printf("  <td>IPアドレス</td>\n");
	printf("  <td>%s</td>\n",empty($DATA['ipaddr']) ? "未割り当て" : preg_replace("/\.0{1,2}/",".",$DATA['ipaddr']));
	printf(" </tr>\n");
    }
    
    if(isset($DATA['ipv6addr'])){
	printf(" <tr>\n");
	printf("  <td>IPv6アドレス</td>\n");
	printf("  <td>%s</td>\n",$DATA['ipv6addr']);
	printf(" </tr>\n");
    }
    printf(" <tr>\n");
    printf("  <td>MACアドレス（有線）</td>\n");
    printf("  <td>%s</d>\n",empty($DATA['macaddr_wired']) ? "&nbsp;":$DATA['macaddr_wired']);
    printf(" </tr>\n");

    printf(" <tr>\n");
    printf("  <td>MACアドレス（無線）</td>\n");
    printf("  <td>%s</td>\n",empty($DATA['macaddr_wireless']) ? "&nbsp;":$DATA['macaddr_wireless']);
    printf(" </tr>\n");

    printf(" <tr>\n");
    printf("  <td>IPv6固定アドレス割り当て</td>\n");
    printf("  <td>%s</td>\n",$DATA['ipv6'] == 't' ? "有効" : "無効");
    printf(" </tr>\n");

    printf(" <tr>\n");
    printf("  <td>MXレコード</td>\n");
    printf("  <td>%s</td>\n", $DATA['mx'] == 't' ? "有効" : "無効" );
    printf(" </tr>\n");
    
    printf(" <tr>\n");
    printf("  <td>備考欄</td>\n");
    printf("  <td>%s</td>\n",empty($DATA['description']) ? "&nbsp;":$DATA['description']);
    printf(" </tr>\n");
    if($mode == "edit"){
	printf(" <tr style=\"text-align:right;\">\n");	
	printf("  <td colspan=\"2\">\n");
	printf("  <form method=\"POST\" style=\"display:inline;\">\n");
	printf("   <input type=\"submit\" value=\"登録\">\n");
	printf("   <input type=\"hidden\" name=\"cmd\" value=\"regist\">\n");
	printf("  </form>\n");

	printf("  <form method=\"POST\" style=\"display:inline;\">\n");
	printf("   <input type=\"submit\" value=\"編集\">\n");
	printf("   <input type=\"hidden\" name=\"cmd\" value=\"edit\">\n");
	printf("  </form>\n");
	printf("  </td>\n");
	printf(" </tr>\n");
    } else if($mode == "delete"){
	printf(" <tr style=\"text-align:right;\">\n");	
	printf("  <td colspan=\"2\">\n");
	printf("  <form method=\"POST\" style=\"display:inline;\">\n");
	printf("   <input type=\"submit\" value=\"削除\">\n");
	printf("   <input type=\"hidden\" name=\"cmd\" value=\"delete\">\n");
	printf("  </form>\n");
	printf("  </td>\n");
	printf(" </tr>\n");
    }
    printf("</table>\n");
}

function printCNAMERecordDATA($DATA = NULL,$mode = NULL){
    global $DOMAIN_LIST;
    printf("<table id=\"edit\">\n");
    printf(" <tr>\n");
    printf("  <th>項目</th>\n");
    printf("  <th>入力情報</th>\n");
    printf(" <tr>\n");
    
    printf(" <tr>\n");
    printf("  <td>別名ホスト名</td>\n");
    printf("  <td>%s</td>\n",$DATA['name']);
    printf(" </tr>\n");
    
    printf(" <tr>\n");
    printf("  <td>参照先ホスト名</td>\n");
    printf("  <td>%s</td>\n",$DATA['rdata']);
    printf(" </tr>\n");
    if($mode == "edit"){
	printf(" <tr style=\"text-align:right;\">\n");	
	printf("  <td colspan=\"2\">\n");
	printf("  <form method=\"POST\" style=\"display:inline;\">\n");
	printf("   <input type=\"submit\" value=\"登録\">\n");
	printf("   <input type=\"hidden\" name=\"cmd\" value=\"regist\">\n");
	printf("  </form>\n");

	printf("  <form method=\"POST\" style=\"display:inline;\">\n");
	printf("   <input type=\"submit\" value=\"編集\">\n");
	printf("   <input type=\"hidden\" name=\"cmd\" value=\"edit\">\n");
	printf("  </form>\n");
	printf("  </td>\n");
	printf(" </tr>\n");
    } else if($mode == "delete"){
	printf(" <tr style=\"text-align:right;\">\n");	
	printf("  <td colspan=\"2\">\n");
	printf("  <form method=\"POST\" style=\"display:inline;\">\n");
	printf("   <input type=\"submit\" value=\"削除\">\n");
	printf("   <input type=\"hidden\" name=\"cmd\" value=\"delete\">\n");
	printf("  </form>\n");
	printf("  </td>\n");
	printf(" </tr>\n");
    }
    printf("</table>\n");
}

function printDescription(){
    printf("<h2>注意事項</h2>\n");
    printf("<ul>\n");
    printf(" <li>MACアドレス（無線）欄と備考欄以外は必ず入力してください。</li>\n");
    printf(" <li>ホスト名は「nirai」のように登録してください．nirai.ie.u-ryukyu.ac.jp のように".
	   " <a href=\"http://e-words.jp/w/FQDN.html\">FQDN</a>を指定する必要はありません．</li>\n");
    printf(" <li>ホスト名に日本語は使えません．</li>\n");
    printf(" <li>IPv6固定アドレス割り当ては，サーバ用途のために使用してください．".
	   "クライアント用途(サーバ以外)の場合，アドレスは無効を選択してください。</li>\n");
    printf(" <li>MXレコードはメールサーバとして使用する場合のみ有効にしてください。</li>\n");
    printf("</ul>\n");
}

function printAdminListbyUser($DNS){
    global $DOMAIN_LIST;
    $ADMIN_LIST  = $DNS->getAdminListbyUser();
    print "<table>\n";
    print " <tr>\n";
    print "  <th>管理者名など</th>\n";
    print "  <th>ドメイン</th>\n";
    if($DOMAIN_LIST['ie']['isEdit'] == true){
	print "  <th>編集</th>\n";
    }
    print " </tr>\n";
    foreach($ADMIN_LIST as $key => $value){
	printf(" <tr>\n");
	printf("  <td>%s(%s)<br ><%s>\n",$value['name'],$key,strMailLink($value['mail'],100));
	printf("  <td style=\"white-space:\">\n");
	if(!empty($value['domain'])){
	    printf("   <ul>\n");
	    foreach($value['domain'] as $domain){
		printf("     <li>%s(%s)</li>\n",$DOMAIN_LIST[$domain]['name'],strtoupper($domain));
	    }
	    printf("   </ul>\n");
	} else {
	    printf("   &nbsp;\n");
	}
	
	printf("  </td>\n");
	if($DOMAIN_LIST['ie']['isEdit'] == true){
	    printf("  <td>\n");
	    printf("    <form method=\"POST\" action=\"%s?cmd=edit\" />\n",$_SERVER['SCRIPT_NAME']);
	    printf("     <input type=\"submit\" value=\"編集\">\n");
	    printf("     <input type=\"hidden\" name=\"sid\"  value=\"%s\" />\n",$key);
	    printf("    </form>\n");	
	    printf("  </td>\n");
	    printf(" </tr>\n");
	}
    }
    print "</table>\n";
}

function printAdminListbyDomain($DNS){
    global $DOMAIN_LIST;
    $ADMIN_LIST  = $DNS->getAdminListbyDomain();
    print "<table>\n";
    print " <tr>\n";
    print "  <th>ドメイン</th>\n";
    print "  <th>管理者名など</th>\n";
    print " </tr>\n";
    foreach($ADMIN_LIST as $key => $value){
	printf(" <tr>\n");
	printf("  <td>%s(%s)</td>\n",$DOMAIN_LIST[$key]['name'],strtoupper($key));
	printf("  <td>\n");
	foreach($value as $key2 => $info){
	    printf("   <ul>%s(%s)<%s></ul>\n",$info['user_name'],$info['sid'],strMailLink($info['mail'],100));
	}
	printf("  </td>\n");
	printf(" </tr>\n");
    }
    print "</table>\n";
}

function printAdminUserEditForm($DATA = NULL,$ERROR = NULL,$mode=null){
    global $DOMAIN_LIST;
    
    printf("<form method=\"POST\">\n");
    printf("<table id=\"edit\">\n");
    print " <tr>\n";
    print "  <th>項目</th>\n";
    print "  <th>記入欄</th>\n";
    print " </tr>\n";

    printf(" <tr %s>\n", isset($ERROR['sid']) ? "id=\"alert\"" : "" );
    printf("  <td>アカウント名</td>\n");
    if($mode != "edit"){
	printf("  <td><input type=\"text\" name=\"sid\" size=\"12\" maxlength=\"12\" value=\"%s\"></td>\n",
	       !empty($DATA['sid']) ? $DATA['sid']: "");
    } else {
	printf("  <td>%s<input type=\"hidden\" name=\"sid\" size=\"12\" maxlength=\"12\" value=\"%s\"></td>\n",
	       !empty($DATA['sid']) ? $DATA['sid']: "",
	       !empty($DATA['sid']) ? $DATA['sid']: "");
    }
    printf(" </tr>\n");
    
    printf(" <tr %s>\n", isset($ERROR['user_name']) ? "id=\"alert\"" : "" );
    printf("  <td>管理者名</td>\n");
    printf("  <td><input type=\"text\" name=\"user_name\" size=\"24\" maxlength=\"24\" value=\"%s\" />\n",
	   !empty($DATA['user_name']) ? $DATA['user_name'] : "");

    printf(" <tr %s>\n", isset($ERROR['mail']) ? "id=\"alert\"" : "" );
    printf("  <td>メールアドレス</td>\n");
    printf("  <td><input type=\"text\" name=\"mail\" size=\"40\" maxlength=\"40\" value=\"%s\" />\n",!empty($DATA['mail']) ? $DATA['mail'] : "");
    printf(" </tr>\n");
    
    printf(" <tr>\n");
    printf("  <td>ドメイン</td>\n");
    printf("  <td>\n");
    foreach($DOMAIN_LIST as $key => $value){
	if(isset($value['name'])){
	    printf("    <input type=\"checkbox\" name=\"domain[%s]\" %s>%s(%s)<br />\n",
		   $key,(isset($DATA['domain'][$key]) && $DATA['domain'][$key] == "on") ? "checked" : "",
		   $value['name'],strtoupper($key));
	}
    }
    printf("  </td>\n");
    printf(" </tr>\n");
    
    printf(" <tr style=\"text-align:right;\">\n");
    printf("  <td colspan=\"2\">\n");
    printf("    <input type=\"submit\"  value=\"送信\" />\n");
    printf("    <input type=\"reset\"   value=\"リセット\" />\n");
    printf("    <input type=\"hidden\"  name=\"cmd\" value=\"regist\" />\n");
    printf("  </td>\n");
    printf(" </tr>\n");

    printf("</table>\n");
}

?>
