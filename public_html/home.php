#!/usr/bin/php
<?php
require_once('path.inc');
require_once('get_host_info.inc');
require_once('rabbitMQLib.inc');
    if ($_POST){
      $username = $_POST["username"];
      $password = hash("sha256", $_POST["password"]);
      //$username = "test";
      //$password = "test";
      $client = new rabbitMQClient("testRabbitMQ.ini","testServer");
      $msg = "Test login";
      $request = array();
      $request['type'] = "login";
      $request['username'] = $username;
      $request['password'] = $password;
      $response = $client->send_request($request);
      echo "client received response: ".PHP_EOL;
      print_r($response);
      echo "\n\n";
      echo $argv[0]." END".PHP_EOL;
    }
?>
