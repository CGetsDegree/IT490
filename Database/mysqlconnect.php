#!/usr/bin/php
<?php
require_once('../rpc/path.inc');
require_once('../get_host_info.inc');
require_once('../rabbitMQLib.inc');
require_once('Connection.php');
require_once('rFunctions.inc');
include('../sendLog.php');

echo SendToLogger("DB Server Startup");

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
      return validateSeassion($request['sessionId']);
    case "logout":S
      return stopSeassion($request['sessionId']);
    case "register":
    	return validateRegister($request['username'],$request['password']);
    case "add_service_list":
    	return addServices($request['username'], $request['serviceID']);
    case "remove_service_list":
    	return removeService($request['username'], $request['serviceID']);
    case "get_service_list":
    	return getServices($request['username']);
    case "add_movie_list":
    	return addServices($request['username'], $request['movieID']);
    case "remove_movie_list":
    	return removeServices($request['username'], $request['movieID']);
    case "get_movie_list":
    	return getMovies($request['username']);
    case "change_movie_rating":
    	return changeRating($request['username'], $request['movieID'], $request['rating']);
    case "get_movie_rating":
    	return getRating($request['username'], $request['movieID']);
    
  }
  return json_encode(array("returnCode" => '0', 'message'=>"Server received request and processed"));
}


$server = new rabbitMQServer("testRabbitMQ.ini","testServer");
echo "server started";
$server->process_requests('requestProcessor');
?>
