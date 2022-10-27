#!/usr/bin/php
<?php
require_once('path.inc');
require_once('get_host_info.inc');
require_once('rabbitMQLib.inc');
require_once('login.php.inc');


echo "successfully connected to database" . PHP_EOL;
function validateLogin($username, $password) {
    $connection = dbconnection();
    $query = "SELECT * FROM users WHERE username = '$username'";
    $result = $connection->query($query);
    echo $result;
    if($result){
    	if($result->num_rows == 0) {
    		return false;
    	}
    	else {
    		while ($row = $result->fetch_assoc()) {
    			if ($row["password"] == $pw)
			{
				echo "passwords match for $username".PHP_EOL;
				return array("returnCode" => '1', 'message'=>"Passwords Match"); // password match
				
			}
			else {
				return array("returnCode" => '0', 'message'=>"Password Does Not Match");
			}
			}
		}
	}
}

function requestProcessor($request)
{
  echo "received request".PHP_EOL;
  var_dump($request);
  if(!isset($request['type']))
  {
    return "ERROR: unsupported message type";
  }
  switch ($request['type'])
  {
    case "login":
      return doLogin($request['username'],$request['password']);
    case "validate_session":
      return doValidate($request['sessionId']);
  }
  return array("returnCode" => '0', 'message'=>"Server received request and processed");
}

$server = new rabbitMQServer("testRabbitMQ.ini","dbServer");
$server->process_requests('requestProcessor');
?>
