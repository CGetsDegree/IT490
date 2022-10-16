#!/usr/bin/php
<?php

$mydb = new mysqli('127.0.0.1','lucast','test123','IT490');

if ($mydb->errno != 0)
{
	echo "failed to connect to database: ". $mydb->error . PHP_EOL;
	exit(0);
}
echo "successfully connected to database" . PHP_EOL;

$query = "select * from testTable;";

$response = $mydb->query($query);
if ($mydb->errno != 0)
{
	echo "failed to execute query: " . PHP_EOL;
	echo __FILE__.':'.__LINE__.":error: ".$mydb->error.PHP_EOL;
	exit(0);
}

$numrows = mysqli_num_rows($response);

echo "We got $numrows row(s) from the query".PHP_EOL;


?>
