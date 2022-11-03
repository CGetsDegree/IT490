<?php
require_once("rabbitFunctions.php");
//var_dump($_GET["topic_id"]);
$request = array("type"=>"get_forum_posts", "forumTopic"=>$_GET["topic_id"]);
$response = sendDB($request);
$response = json_decode($response,true);
//var_dump($response);
$title = $response["forumTitle"];
$id=$_GET["topic_id"];
$display_block = "
	<P>Showing comments for the <strong>$title</strong> Post:</p>
	<table width=100% cellpadding=3 cellspacing=1 border=1>
	<tr>
	<th>AUTHOR</th>
	<th>COMMENT</th>
	</tr>";
foreach($response["forumArray"] as $comment){
	$comment_id = $comment[0];
	$comment_text = nl2br(stripslashes($comment[1]));
	$comment_date = $comment[2];
	$comment_owner = $comment[3];
	$display_block .= "
		<tr>
		<td width=35% valign=top>$comment_owner<br>[$comment_date]</td>
		<td width=65% valign=top>$comment_text<br><br>
		<a href=\"replyToPost.php?post_id=$comment_id&topic_id=$id\"><strong>REPLY TO POST</strong></a></td>
		</tr>";
}
$display_block .= "</table>";
?>
<html>
<head>
<title>Comments on this Post</title>
</head>
<body>
<h1>Comments on this Post</title>
<?php print $display_block; ?>
</body>
</html>
