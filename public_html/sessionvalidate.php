<?php
require_once('path.inc');
require_once('get_host_info.inc');
require_once('rabbitMQLib.inc');
if ($_POST){
      $rtype=$_POST["type"];
      $sessionid = $_POST["sessionid"];
      $client = new rabbitMQClient("testRabbitMQ.ini","testServer");
      $request = array();
      $request['type'] = $rtype;
      $request['sessionid'] = $sessionid;
      $response = $client->send_request($request);
      header("Content-Type: application/json");
      echo $response;
    }
    else{
    	header("Location:login.html");
    }
?>
