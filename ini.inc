<?php
$version = "v0.2b";
$title = "情報工学科IPアドレス申請ページ " . $version;

$DHCP_CMD   = dirname($_SERVER['SCRIPT_FILENAME']) . "/dhcp_cmd/dhcprequest";
$DHCP_SERVER = "127.0.0.1";
$DHCP_PORT  = 10004;
$DHCP_USE   = true;

$PG_HOST = "133.13.48.3"; /* ホスト名 */
$PG_PORT = "5432";        /* ポート番号 */ 
$PG_DB   = "named";       /* データベース名 */
$PG_USER = "named";       /* ユーザ名 */
$PG_PASS = "root58330";   /* パスワード*/


/* 管理者 メールアドレス */
//$ADMIN_MAIL = "host-regist@ie.u-ryukyu.ac.jp";
$SEND_FROM  = "host-regist@ie.u-ryukyu.ac.jp";
$ADMIN_MAIL = "host-regist@ie.u-ryukyu.ac.jp";

$LDAP_SERVER  = "133.13.48.2";
$LDAP_BASE_DN = "ou=ie,o=u-ryukyu,c=jp";
$LDAP_BIND_DN = "cn=Manager,ou=ie,o=u-ryukyu,c=jp";

$LDAP_PASS = "ryu-ldap-me";

$NUM_RECORD = 30;
?>
