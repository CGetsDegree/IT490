#!/usr/bin/php
<?php
require_once('path.inc');
require_once('get_host_info.inc');
require_once('rabbitMQLib.inc');

$mydb = new mysqli('127.0.0.1','kepsin','12345','IT490');

if ($mydb->errno != 0)
{
	echo "failed to connect to database: ". $mydb->error . PHP_EOL;
	exit(0);
}
echo "successfully connected to database" . PHP_EOL;
function validateLogin($username, $password) {
    $connection = dbconnection();
    $query = "SELECT * FROM users WHERE username = '$username'";
    $result = $connection->query($query);
    if($result){
    	if($result->num_rows == 0) {
    		return false;
    	}
    	else {
    		while ($row = $result->fetch_assoc()) {
    			if ($row["password"] == $pw)
			{
				echo "passwords match for $username".PHP_EOL;
				return array("returnCode" => '1', 'message'=>"Passwords Match");// password match
				return true;
			}
			else {
				return false;
			}
			}
		}
	}
}

$server = new rabbitMQServer("testRabbitMQ.ini","testServer");
$server->process_requests('requestProcessor');
?>
