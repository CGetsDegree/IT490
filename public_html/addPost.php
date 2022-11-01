<?php
require_once("rabbitFunctions.php");
//echo $_POST["post_title"] . ": " . $_POST["post_text"];
//	echo $_COOKIE["username"] . " made the following post:\n";
//	echo $_POST["post_title"] . ": " . $_POST["post_text"];
$forumPost = array(
	"type"=>"addPost",
	"username"=>"test",
	"post_title"=>$_POST["post_title"],
	"post_text"=>$_POST["post_text"]);
sendDB(json_encode($forumPost));
?>
