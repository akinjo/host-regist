<?php
$title = "琉球大学 工学部 情報工学科 DNS,DHCP 登録ページ";

$DHCP_CMD   = dirname($_SERVER['SCRIPT_FILENAME']) . "/cmd/dhcprequest";
$DHCP_SEVER = "127.0.0.1";
$DHCP_PORT  = 10004;

/* ホスト名 */
$PG_HOST = "futenma.ie.u-ryukyu.ac.jp";

/* ポート番号 */
$PG_PORT = "5432";

/* データベース名 */     
$PG_DB   = "bind";

/* テーブル名 */
$PG_TBL  = "base";  

/* ユーザ名 */
$PG_USER = "named"; 

$ADMIN_USER = "akinjo";

/* パスワード*/
$PG_PASS = "root58330";

/* 管理者 メールアドレス */
$ADMIN_MAIL = "host-regist@ie.u-ryukyu.ac.jp";
$SEND_FROM  = "host-regist@ie.u-ryukyu.ac.jp";

/* 所属 リスト*/

$DOMAIN_LIST = array("st"     => "st(学部生)",
		     "iip"    => "iip(高良研)",
		     "dsp"    => "dsp(アシャリフ研)",
		     "eva"    => "eva(遠藤・山田研)",
		     "nal"    => "nal(當間研)",
		     "tea"    => "tea(赤嶺研)",
		     "lsi"    => "lsi(和田研(舟木研))",
		     "engr"   => "engr(長山研)",
		     "cr"     => "cr(河野研)",
		     "fts"    => "fts(吉田研)");
/*
		     "cs"     => "クラスタ(CS)",
		     "neo"    => "neo(玉城・長田研)",
		     "ads"    => "ads(名嘉村研)",
		     "ms"     => "ms(岡崎研(谷口研))",
		     "sys"    => "sys(宮城・姜研)",
		     "nc"     => "nc(宮里研)");
*/
?>

