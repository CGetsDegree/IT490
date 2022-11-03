<?php

require_once("Connection.php");

/*
* returns all of the posts in a topic
*/
function sendForumPosts($forumTopic) {
	$sparray = array();
	$forumarray = array();
	//$textArray = array();
	//$timeArray = array();
	//$ownerArray = array();
	$logindb = databaseConnect();
	$ft = $logindb->real_escape_string($forumTopic);
	$query = "select * from forum_post, forum_topics where forumId = $forumTopic and topicId = $forumTopic";
	$select = $logindb->query($query);
	while($row = $select->fetch_assoc()) {
		$title = $row["topicTitle"];
		if($row["forumId"] == $forumTopic) {
			$sparray[] = $row["postId"];
			array_push($sparray, $row["postText"]);
			array_push($sparray, $row["postCreateTime"]);
			array_push($sparray, $row["postOwner"]);
			array_push($forumarray, $sparray);
			unset($sparray);
		}
	}
	return json_encode(array("returnCode" => '1', 'message' => "forum post array created", 'forumArray'=>$forumarray, 'forumTitle'=>$title));
	//, 'postTextArray'=>$textArray, 'postTimeArray'=>$timeArray, 'postOwnerArray'=>$ownerArray);	
}

/*
* returns all of the topics in the forum
*/
function sendForumTopics() {
	$sparray = array();
	$forumArray = array();
	//$textArray = array();
	//$timeArray = array();
	//$ownerArray = array();
	$logindb = databaseConnect();
	$query = "select * from forum_topics";
	$select = $logindb->query($query);
	while($row = $select->fetch_assoc()) {
		$sparray[] = $row["topicId"];
		array_push($sparray, $row["topicTitle"]);
		array_push($sparray, $row["topicCreateTime"]);
		array_push($sparray, $row["topicOwner"]);
		$tp = $row["topicId"];
		$sels = "select count(*) from forum_post where forumId='$tp'";
		$sel = $logindb->query($sels);
		while($ro = $sel->fetch_assoc()) {
			array_push($sparray, $ro["count(*)"]);
		}
		array_push($forumArray, $sparray);
		unset($sparray);
	}
	return json_encode(array("returnCode" => '1', 'message' => "forum post array created", 'forumArray'=>$forumArray));//, 'topicTitleArray'=>$textArray, 'topicTimeArray'=>$timeArray, 'topicOwnerArray'=>$ownerArray);	
}

/*
* creates a forum topic in database
*/
function createForumTopic($username, $forumName, $postText) {
	$logindb = databaseConnect();
	$uid = $logindb->real_escape_string($username);
	$fn = $logindb->real_escape_string($forumName);
	$query = "insert into forum_topics values(0, '$fn', current_timestamp, '$uid')";
	$insert = $logindb->query($query);
	if ($insert === TRUE) {
		$sel = "select MAX(topicId) from forum_topics";
		$select = $logindb->query($sel);
		while($row = $select->fetch_assoc()) {
			echo "what is this id  = ".$row["MAX(topicId)"].PHP_EOL;
			$forum = createForumPost($username, $postText, $row["MAX(topicId)"]);
			return json_encode(array("returnCode" => '1', 'message' => "Topic created successfully"));
		}
			
	}
	return json_encode(array("returnCode" => '0', 'message' => "failed to create topic"));
}

/*
* creates a forum post in database
*/
function createForumPost($username, $postText, $forumID) {
	$logindb = databaseConnect();
	$uid = $logindb->real_escape_string($username);
	$fid = $logindb->real_escape_string($forumID);
	$pt = $logindb->real_escape_string($postText);
	$query = "insert into forum_post values(0, '$fid', '$pt', current_timestamp, '$uid')";
	if ($logindb->query($query) === TRUE) {
		return json_encode(array("returnCode" => '1', 'message' => "post created sucessfully"));	
	}
	return json_encode(array("returnCode" => '0', 'message' => "failed to created post"));
}

?>
