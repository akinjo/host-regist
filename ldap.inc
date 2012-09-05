<?php


class LDAP {
    var $ld;
    var $base_dn;
    function LDAP($LDAP_SERVER,$LDAP_BASE_DN,$LDAP_BIND_DN,$LDAP_PASS){
	$this->ld = ldap_connect($LDAP_SERVER);
	ldap_set_option($this->ld, LDAP_OPT_PROTOCOL_VERSION, 3);
	ldap_bind($this->ld,$LDAP_BIND_DN,$LDAP_PASS);
	$this->base_dn = $LDAP_BASE_DN;
    }
    
    function getUserDATA($user){
	$filter = sprintf("(&(objectClass=*)(uid=%s))",$user);
	$sres   = ldap_search($this->ld, $this->base_dn,$filter);
	$count  = ldap_count_entries($this->ld, $sres);
	$info   = ldap_first_entry($this->ld, $sres);
	$attrs  = ldap_get_attributes($this->ld, $info);
	return($attrs);
    }

    function isSIDExist($sid){
	$filter = sprintf("(&(objectClass=*)(uid=%s))",$sid);
	$sres   = ldap_search($this->ld, $this->base_dn,$filter);
	$count  = ldap_count_entries($this->ld, $sres);
	
	if($count > 0){
	    return(true);
	} else {
	    return(false);
	}
    }
}
