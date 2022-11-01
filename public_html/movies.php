<?php
require_once('path.inc');
require_once('get_host_info.inc');
require_once('rabbitMQLib.inc');
require_once('rabbitFunctions.php');
if ($_POST){
      $rtype=$_POST["type"];
      $sessionid = $_POST["sessionid"];
      $searchquery=$_POST["searchquery"];
      $movies=array();
      if ($_POST["movies"]){
	      $movies=json_decode($_POST["movies"]);
      }
      $request = array();
      $request['type'] = $rtype;
      $request['sessionid'] = $sessionid;
      if ($rtype=="search"){
      	$request['query']=$searchquery;
	$request['page']=$page;
      }
      else if ($rtype=="addToWatchlist"){
	      $request["movies"]=$movies;
      }
      $response = sendAPI($request);
      echo $response;
    }
    else{
    	header("Location:login.html");
    }
?>
