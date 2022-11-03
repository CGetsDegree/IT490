<?php
require_once('path.inc');
require_once('get_host_info.inc');
require_once('rabbitMQLib.inc');
if ($_POST){
      $rtype=$_POST["type"];
      $sessionid = $_POST["sessionid"];
      $request = array();
      $request['type'] = $rtype;
      $request['sessionid'] = $sessionid;
      if ($rtype=="search"){
      	$request['query']=$_POST["searchquery"];
	$request['page']=$_POST["page"];
      }
     else if ($rtype=="add_movie_list"){
	      $movies=array();
	      $movies=json_decode($_POST["movies"]);
	      $request["movies"]=$movies;
         }
      $client=new rabbitMQClient("testRabbitMQ.ini","testServer");
      $response=$client->send_request($request);
      header("Content-Type: application/json");
      echo $response;
    }
    else{
    	echo "BAD";
    }
?>
