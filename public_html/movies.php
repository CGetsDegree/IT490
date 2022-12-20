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
      $request["username"]=$_POST["username"];
      if ($rtype=="search"){
      	$request['query']=$_POST["searchquery"];
	$request['page']=$_POST["page"];
      }
     else if ($rtype=="add_movie_list"){
	     $request["movieid"]=$_POST["movie_id"];
	     $request["title"]=$_POST["title"];
     }
     else if ($rtype=="change_movie_rating"){
	     $request["movieid"]=$_POST["movie_id"];
	     $request["rating"]=$_POST["rating"];
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
