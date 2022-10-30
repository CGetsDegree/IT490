#!/usr/bin/php
<?php
require_once('path.inc');
require_once('get_host_info.inc');
require_once('rabbitMQLib.inc');

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
    case "search":
      return doLogin($request['username'],$request['password']);
    case "get_details":
      return doValidate($request['sessionId']);
    case "get_recommendation":
      return doValidate($request['sessionId']);
    case "get_watchlist":
      return doValidate($request['sessionId']);  }
  return array("returnCode" => '0', 'message'=>"Server received request and processed");
}

$server = new rabbitMQServer("testRabbitMQ.ini","dmz");

echo "Server started up".PHP_EOL;
$server->process_requests('requestProcessor');
echo "Server shut down".PHP_EOL;
exit();
?>

?>
