<?php
require_once("rabbitFunctions.php");
//echo $_POST["post_title"] . ": " . $_POST["post_text"];
//	echo $_COOKIE["username"] . " made the following post:\n";
//	echo $_POST["post_title"] . ": " . $_POST["post_text"];
//$client = new rabbitMQClient("testRabbitMQ.ini","testServer");
$name = $_POST["post_title"];
$forumPost = array(
	"type"=>"create_forum_topic",
	"username"=>"test",
	"forumName"=>$_POST["post_title"],
	"postText"=>$_POST["post_text"]);
//var_dump($forumPost);
//$response = $client->send_request($forumPost);
//echo json_decode($response);
//var_dump($forumPost);
$response = sendDB($forumPost);
var_dump($response);
$msg = "<P> The <strong>$name</strong> topic has been created!</p>";
?>
<html>
<head>
<title>New Topic Added</title>
</head>
<body>
<h1>New Topic Added</h1>
<?php print $msg ?>
</body>
</html>
