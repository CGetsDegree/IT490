#!/usr/bin/php
<?php

$mydb = new mysqli('192.168.192.221','kepsin','12345','IT490');

if ($mydb->errno != 0)
{
	echo "failed to connect to database: ". $mydb->error . PHP_EOL;
	exit(0);
}
echo "successfully connected to database" . PHP_EOL;

$query = "select * from users;";

$response = $mydb->query($query);
if ($mydb->errno != 0)
{
	echo "failed to execute query: " . PHP_EOL;
	echo __FILE__.':'.__LINE__.":error: ".$mydb->error.PHP_EOL;
	exit(0);
}


?>
