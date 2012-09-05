<?php
include_once("./ini.inc");

class DHCP {
    var $server;
    var $port;
    function DHCP($server,$port){
	$this->server = $server;
	$this->port   = $port;
    }
    
    function insert($host,$ip,$mac){
	global $DHCP_CMD;
	$cmd = sprintf("%s -s %s -p %d create %s %s %s",
		       $DHCP_CMD,$this->server,$this->port, $host,$ip,$mac);
	system($cmd,$ret);
	return($ret);
    }
    
    function delete($host){
	global $DHCP_CMD;
	$cmd = sprintf("%s -s %s -p %d remove %s",
		       $DHCP_CMD,$this->server,$this->port,$host);
	system($cmd,$ret);
	return($ret);
    }
    
    function update($host,$ip,$mac){
    }
}

?>