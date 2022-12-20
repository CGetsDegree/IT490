<?php
require_once('path.inc');
require_once('get_host_info.inc');
require_once('rabbitMQLib.inc');
if ($_POST){
      $rtype=$_POST["requestType"];
      $sessionid = $_POST["sessionid"];
      $request = array();
      $request['type'] = $rtype;
      $request['sessionid'] = $sessionid;
      $request["username"]=$_POST["username"];
      $request["id"]=$_POST["user_id"];
      $client=new rabbitMQClient("testRabbitMQ.ini","testServer");
      $response=$client->send_request($request);
      header("Content-Type: application/json");
      echo $response;
    }
    else{
        echo "BAD";
    }
?>

