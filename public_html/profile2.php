<?php
require_once('rabbitFunctions.php');
$user=$_GET["u"];
$request=array("type"=>"get_user_watchlist","user"=>$user);
$response=sendDB($request);
$response=json_decode($response,true);
$display_block="<table><th><td>Title</td><td>Release date</td><td>Streaming on</td></th>";
foreach($response as $r){
	$id=$r["id"];
	$title=$r["title"];
	$release_date=$r["release_date"];
	$streams=$r["streaming_services"];
	$display_block.="<tr><td><a href='movieDetail.php?id=$id'>$title</a></td><td>$release_date</td>";

}
