<?php
require_once("rabbitFunctions.php");
$request = array("type"=>"listPosts");
$response = sendDB(json_encode($request));
$response = json_decode($response, true);
$display_block = "
	<table cellpadding=3 cellspacing=1 border=1>
	<tr>
	<th>POST TITLE</th>
	<th>Number of Comments</th>
	</tr>";
foreach($response as $post){
	$post_id = $post["topic_id"];
	$post_title = $post["topic_title"];
	$post_create_time = $post["topic_create_time"];
	$post_owner = $post["topic_owner"];
	$num_comments = $post["num_posts"];
	
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

