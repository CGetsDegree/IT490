#!/usr/bin/php
<?php
require_once('path.inc');
require_once('get_host_info.inc');
require_once('rabbitMQLib.inc');
require("apiStuff.php");
include('sendLog.php');

//$request = array("type"=>"search", "query"=>"Star Wars", "page"=>1);

echo SendToLogger("API Server Startup");

function requestProcessor($request)
{
  //$request = json_decode($request,true);
  echo "received request".PHP_EOL;
  var_dump($request);
  if(!isset($request['type']))
  {
    return "ERROR: unsupported message type";
  }
  switch ($request['type'])
  {
    case "search":
    	if(isset($request["page"])){
    		//echo "page is set";
    		return json_encode(searchMovies($request['query'],$request['page']));
    	}
    	else{
    		//echo "page not set, returning page 1 as default";
    		return json_encode(searchMovies($request['query']));
    	}
     
    case "get_details":
      return getMovieDetails($request['id']);
    case "get_recommendation":
      return getRecommended($request['id']);
    case "get_watchlist":
      return displayWatchlist($request['movies']);  }
  //return array("returnCode" => '0', 'message'=>"Server received request and processed");
}

$server = new rabbitMQServer("testRabbitMQ.ini","dmz");

echo "Server started up".PHP_EOL;
$server->process_requests('requestProcessor');
echo "Server shut down".PHP_EOL;
exit();
?>

?>
