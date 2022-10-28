<?php

require_once('Connection.php');

function validateLogin($username,$password)
	{
		$logindb = databaseConnect();
		$un = $logindb->real_escape_string($username);
		$pw = $logindb->real_escape_string($password);
		$statement = "select * from users where username = '$un'";
		$response = $logindb->query($statement);

		while ($row = $response->fetch_assoc())
		{
			echo "checking password for $username".PHP_EOL;
			if ($row["password"] == $pw)
			{
				echo "passwords match for $username".PHP_EOL;
				return json_encode(array("returnCode" => '1', 'message'=>"Passwords Match"));// password match
			}
			echo "passwords did not match for $username".PHP_EOL;
		}
		
		return json_encode(array("returnCode" => '0', 'message'=>"No Users Match Username"));//no users matched username
	}
	
function validateRegister($username,$password)
	{
		$logindb = databaseConnect();
		$un = $logindb->real_escape_string($username);
		$pw = $logindb->real_escape_string($password);
		$statement = "select * from users where username = '$un'";
		$response = $logindb->query($statement);

		while ($row = $response->fetch_assoc())
		{
			echo "checking usernam for $username".PHP_EOL;
			if ($row["username"] == $un)
			{
				echo "username already exist".PHP_EOL;
				return json_encode(array("returnCode" => '0', 'message'=>"Username already exist try again"));// password match
			}
		}
		$insert = "insert into users values(0, '$un', '$pw')";
		if ($logindb->query($insert) === TRUE) {
			return json_encode(array("returnCode" => '1', 'message'=>"Username and password accepted"));
		}
		return json_encode(array("returnCode" => '0', 'message'=>"something went wrong try again")); 
	}

function createSession($sessionId) {
	$logindb = databaseConnect();
	$session = $logindb->real_escape_string($sessionId);
	$query = "select * from sessions where sessionsID = '$session'";
	$response = $logindb->query($query);
	while ($row = $response->fetch_assoc()) {
		if ($row["sessionID"] == $session) {
			return json_encode(array("returnCode" => '0', 'message' => "SessionID already exist"));
		} 
	}
	$insert = "insert into sessions (sessionID, creationTime, expireTime) values('$session', CURRENT_TIME(), ADDTIME(CURRENT_TIME(), 020000))";
	if($logindb->query($insert) === TRUE) {
		return json_encode(array("returnCode" => '1', 'message' => "SessionID is created"));
	}
	return json_encode(array("returnCode" => '0', 'message' => "SessionID is invalid"));
}	
	
function validateSession($sessionId) {
	$logindb = databaseConnect();
	$session = $logindb->real_escape_string($sessionId);
	$query = "select * from sessions where sessionsID = '$session'";
	$response = $logindb->query($query);
	while ($row = $response->fetch_assoc()) {
		if ($row["sessionID"] == $session) {
			return json_encode(array("returnCode" => '1', 'message' => "SessionID is valid"));
		} 
	}
	return json_encode(array("returnCode" => '0', 'message' => "SessionID is invalid"));
}
?>
