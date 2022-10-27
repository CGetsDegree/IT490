<?php
require_once('path.inc');
require_once('get_host_info.inc');
require_once('rabbitMQLib.inc');
if ($_POST){
      $rtype=$_POST["type"];
      $username = $_POST["username"];
      $password = hash("sha256", $_POST["password"]);
      //$username = "test";
      //$password = "test";
      $client = new rabbitMQClient("testRabbitMQ.ini","testServer");
      $request = array();
      $request['type'] = $rtype;
      $request['username'] = $username;
      $request['password'] = $password;
      $response = $client->send_request($request);
      header("Content-Type: application/json");
      echo $response;
    }
    else{
    	header("Location:login.php");
    }
?>