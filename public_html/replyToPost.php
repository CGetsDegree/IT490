<html>
<head>
<title>Post your reply in this topic</title>
</head>
<body>
<h1>Post your reply in this topic</h1>
<form method=post action=\"$_SERVER[PHP_SELF]\">

<p><strong>Reply Text:</strong><br>
<textarea name=\"reply_text\" rows=8 cols=40 wrap=virtual></textarea>

<input type=\"hidden\" name=\"op\" value=\"addpost\">
<input type=\"hidden\" name=\"topic_id\" value=\$id\">

<P><input type=\"submit\" name=\"submit\" value=\"Add Reply\"></p>

</form>
</body>
</html>
<?php
require_once("rabbitFunctions.php");
var_dump($_GET["topic_id"]);
$request = array("type"=>"get_forum_posts", "forumTopic"=>$_GET["topic_id"]);
//$response = sendDB($request);
var_dump($response);
//$response = json_decode($response,true);
//$title = $response["forumTitle"];
$id = $_GET["topic_id"];
$replyRequest = array("type"=>"create_forum_post", "username"=>$_COOKIE["username"], "topic_id"=>$id, "postText"=>$_POST["reply_text"]);
$response = sendDB($replyRequest);
var_dump($response);
header("Location: showTopic.php?topic_id=$id");
exit();
?>
