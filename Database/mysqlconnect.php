#!/usr/bin/php
<?php
require_once('../path.inc');
require_once('../get_host_info.inc');
require_once('../rabbitMQLib.inc');
require_once('Connection.php');
require_once('rFunctions.inc');


function requestProcessor($request)
{
  echo "received request".PHP_EOL;
  var_dump($request);
  if(!isset($request['type']))
  {
    return "ERROR: unsupported message type";
  }
  switch ($request['type'])
  {
    case "login":
      return validateLogin($request['username'],$request['password']);
    case "validate_session":
      return doValidate($request['sessionId']);
    case "register":
    	return validateRegister($request['username'],$request['password']);
  }
  return array("returnCode" => '0', 'message'=>"Server received request and processed");
}


$server = new rabbitMQServer("testRabbitMQ.ini","testServer");
echo "server started";
$server->process_requests('requestProcessor');
?>
