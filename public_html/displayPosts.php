<?php
require_once("rabbitFunctions.php");
$request = array("type"=>"get_forum_topics");
$response = sendDB($request);
$response = json_decode($response, true);
//var_dump($response);
$display_block = "
	<table cellpadding=3 cellspacing=1 border=1>
	<tr>
	<th>POST TITLE</th>
	<th>Number of Comments</th>
	</tr>";
foreach($response["forumArray"] as $post){
	$post_id = $post[0];
	$post_title = $post[1];
	$post_create_time = $post[2];
	$post_owner = $post[3];
	$num_comments = $post[4];
	
	$display_block .= "
		<tr>
		<td><a href=\"showTopic.php?topic_id=$post_id\">
		<strong>$post_title</strong></a><br>
		Created on $post_create_time by $post_owner</td>
		<td align=center>$num_comments</td>
		</tr>";
}
$display_block .= "</table>";
?>
<html>
<head>
<title>All Posts</title>
</head>
<body>
<h1>All Posts</h1>
<?php print $display_block; ?>
<P>Would you like to <a href="makePost.html"> add a Post</a>?</p>
</body>
</html>

