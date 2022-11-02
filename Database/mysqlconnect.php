#!/usr/bin/php
<?php
require_once('../rpc/path.inc');
require_once('../get_host_info.inc');
require_once('../rabbitMQLib.inc');
require_once('Connection.php');
require_once('rFunctions.inc');
require_once('forumFunctions.inc');


function requestProcessor($request)
{
  echo "received request".PHP_EOL;
  var_dump($request);
  //$request = json_decode($request, true);
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
      return validateSession($request['sessionid']);
    case "logout":
      return stopSession($request['sessionid']);
    case "register":
    	return validateRegister($request['username'],$request['password']);
    case "add_service_list":
    	return addServices($request['username'], $request['serviceid']);
    case "remove_service_list":
    	return removeService($request['username'], $request['serviceid']);
    case "get_service_list":
    	return getServices($request['username']);
    case "add_movie_list":
    	return addServices($request['username'], $request['movieid']);
    case "remove_movie_list":
    	return removeServices($request['username'], $request['movieid']);
    case "get_movie_list":
    	return getMovies($request['username']);
    case "change_movie_rating":
    	return changeRating($request['username'], $request['movieid'], $request['rating']);
    case "get_movie_rating":
    	return getRating($request['username'], $request['movieid']);
    case "get_forum_posts":
    	return sendForumPosts($request['forumTopic']);
    case "get_forum_topics":
    	return sendForumTopics();
    case "create_forum_topic":
    	return createForumTopic($request['username'], $request['forumName'], $request['postText']);
    case "create_forum_post":
    	return createForumPost($request['username'], $request['postText'], $request['forumTopic']);
    
  }
  return json_encode(array("returnCode" => '0', 'message'=>"Server received request and processed"));
}


$server = new rabbitMQServer("testRabbitMQ.ini","testServer");
echo "server started".PHP_EOL;
$server->process_requests('requestProcessor');
?>
