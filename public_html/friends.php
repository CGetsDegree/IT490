<?php
require_once('path.inc');
require_once('get_host_info.inc');
require_once('rabbitMQLib.inc');
if ($_POST){
      $request = array();
      $request['type'] = $rtype;
      $request['sessionid'] = $_POST["session_id"];
      $request["username"]=$_POST["username"];
      $request["type"]=$_POST["request_type"];
      $request["friendcode"]=$_POST["friend_code"];
      
      $client=new rabbitMQClient("testRabbitMQ.ini","testServer");
      $response=$client->send_request($request);
      header("Content-Type: application/json");
      echo $response;
    }
    else{
    	echo "No POST request found";
    }
?>
