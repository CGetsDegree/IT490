#!/usr/bin/php
<?php
require_once('rabbitFunctions.php');

if (isset($argv[1]))
{
  $msg = $argv[1];
}
else
{
  $msg = "Suck my balls Lucas";
}

$request = array();
$request['type'] = "Login";
$request['username'] = "Frank";
$request['password'] = "yourmom";
$request['message'] = $msg;
$response = sendDB($request);
//$response = $client->publish($request);

echo "client received response: ".PHP_EOL;
print_r($response);
echo "\n\n";

echo $argv[0]." END".PHP_EOL;

