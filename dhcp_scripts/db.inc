<?php
include_once("./ini.inc");

class PGSQL_DB {
  var $db;
  var $ret;

  function PGSQL_DB(){
    global $PG_HOST;
    global $PG_PORT;
    global $PG_DB;
    global $PG_USER;
    global $PG_PASS;
    
    $str = sprintf("dbname=%s port=%s host=%s user=%s password=%s",
		   $PG_DB,$PG_PORT,$PG_HOST,$PG_USER,$PG_PASS);
    $this->db = pg_connect($str);
  }

  function query($query){
    $this->ret = pg_query($this->db,$query);
  }
  
  function get(){
    $array =  pg_fetch_array($this->ret);
    return($array);
  }

  function escape($DATA){
    foreach ($DATA as $key => $value){
      $DATA[$key] = pg_escape_string($value);
    }
  }
}
?>
