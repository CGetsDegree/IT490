<?php
require_once("rabbitFunctions.php");
var_dump($_POST["post_id"]);
$request = array("type"=>"get_forum_posts", "forumTopic"=>$_POST["post_id"]);
$respons = sendDB($request);
?>
