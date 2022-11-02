<?php
require_once("rabbitFunctions.php");
//echo $_POST["post_title"] . ": " . $_POST["post_text"];
//	echo $_COOKIE["username"] . " made the following post:\n";
//	echo $_POST["post_title"] . ": " . $_POST["post_text"];
//$client = new rabbitMQClient("testRabbitMQ.ini","testServer");
$forumPost = array(
	"type"=>"create_forum_topic",
	"username"=>"test",
	"forumName"=>$_POST["post_title"],
	"postText"=>$_POST["post_text"]);
//var_dump($forumPost);
//$response = $client->send_request($forumPost);
//echo json_decode($response);
sendDB(json_encode($forumPost));
?>
