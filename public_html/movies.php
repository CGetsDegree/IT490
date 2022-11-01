<?php
require_once('rabbitFunctions.php');
if ($_POST){
      $rtype=$_POST["type"];
      $sessionid = $_POST["sessionid"];
      $searchquery=$_POST["searchquery"];
      $request = array();
      $request['type'] = $rtype;
      $request['sessionid'] = $sessionid;
      if ($rtype=="search"){
      	$request['query']=$searchquery;
	$request['page']=$page;
      }
      else if ($rtype=="addToWatchlist"){
	      $movies=array();
	      $movies=json_decode($_POST["movies"])
	      $request["movies"]=$movies;
      }
      $response = sendAPI($request);
      echo $response;
    }
    else{
    	header("Location:login.html");
    }
?>
