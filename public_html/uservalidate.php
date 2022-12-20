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
      if (isset($_POST["email"])){
	      $request["email"]=$_POST["email"];
      }
      $response = $client->send_request($request);
      echo $response;
    }
    else{
	    $r=array();
	    $r["message"]="error";
	    echo json_encode($r);
    }
?>
