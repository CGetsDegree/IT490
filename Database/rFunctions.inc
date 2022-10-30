<?php

require_once('Connection.php');

/*
* Goes through the database and looks to see if the login credentials are valid for login.
*/
function validateLogin($username,$password)
	{
		$logindb = databaseConnect(); 
		$un = $logindb->real_escape_string($username);
		$pw = $logindb->real_escape_string($password);
		$statement = "select * from users where username = '$un'";
		$response = $logindb->query($statement);

		while ($row = $response->fetch_assoc()) //goes through the users in table
		{
			echo "checking password for $username".PHP_EOL;
			if ($row["password"] == $pw)
			{
				echo "passwords match for $username".PHP_EOL;
				$sessionid = createSession(rand(100000, 99999999));
				if ($sessionid != NULL) {
					return json_encode(array("returnCode" => '1', 'message'=>"Passwords Match", 'sessionId'=>'$sessionid'); // password matched with username, allows login
				}
				else {
					return json_encode(array("returnCode" => '0', 'message'=>"Error generating session"));
				}
				
			}
			echo "passwords did not match for $username".PHP_EOL; //passwords didn't match, continues through list of users.
		}
		
		return json_encode(array("returnCode" => '0', 'message'=>"No Users Match Username")); //no users matched username, no login
	}
	
/*
* Creates a new profile and checks to see if username is taken already.
* Puts the profile in the table users.
*/		
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
				return json_encode(array("returnCode" => '0', 'message'=>"Username already exist try again")); //username is already being used 
			}
		}
		$insert = "insert into users values(0, '$un', '$pw')"; //creates a new user and password in users table
		if ($logindb->query($insert) === TRUE) { //check to make sure it worked
			return json_encode(array("returnCode" => '1', 'message'=>"Username and password accepted")); //returns that it worked to user
		}
		return json_encode(array("returnCode" => '0', 'message'=>"something went wrong try again")); //something went wrong in the registration of the user.
	}
/*
* Creates a sessionid for the user when logged in. 
* Puts the sessionid in sessions table with expiration time and time of creation.
*/
function createSession($sessionId) {
	$logindb = databaseConnect();
	$session = $logindb->real_escape_string($sessionId);
	$query = "select * from sessions where sessionsID = '$session'";
	$response = $logindb->query($query);
	while ($row = $response->fetch_assoc()) {
		if ($row["sessionID"] == $session) {
			return NULL; //sessionID already exist and that shouldn't be
		} 
	}
	$insert = "insert into sessions (sessionID, creationTime, expireTime) values('$session', CURRENT_TIME(), ADDTIME(CURRENT_TIME(), 020000))"; //creates a new sessionID in tables sessions
	if($logindb->query($insert) === TRUE) { //checks to make sure it worked
		return $sessionId;
	return NULL;
}	
	
/*
* Checks to make sure a logged in user has a valid session id and will log them out if not.
*/	
function validateSession($sessionId) {
	$logindb = databaseConnect();
	$session = $logindb->real_escape_string($sessionId);
	$query = "select * from sessions where sessionsID = '$session' AND expireTime > current_time()"; //selects all current sessions that aren't expired
	$response = $logindb->query($query);
	while ($row = $response->fetch_assoc()) {
		if ($row["sessionID"] == $session) {
			return json_encode(array("returnCode" => '1', 'message' => "SessionID is valid")); //returns a valid sessionID and allows the user to stay logged in.
		} 
	}
	stopSession($sessionId); //stops the session if it no longer is valid.
	return json_encode(array("returnCode" => '0', 'message' => "SessionID is invalid"));
}

/*
* Allows the user to logout and deletes the sessionID from sessions table.
*/
function stopSession($sessionId) {
	$logindb = databaseConnect();
	$session = $logindb->real_escape_string($sessionId);
	$query = "select * from sessions where sessionsID = '$session'";
	$response = $logindb->query($query);
	while ($row = $response->fetch_assoc()) {
		if ($row["sessionID"] == $session) {
			$delete = "delete from sessions where sessionID='$session'"; //deletes the session from the table sessions
		f($logindb->query($delete) === TRUE) { //checks to make sure it worked
			return json_encode(array("returnCode" => '1', 'message' => "Logout successfull")); //returns if logout worked 
		} 
		return json_encode(array("returnCode" => '0', 'message' => "already logged out")); //returns when user is already logged out
	}
	
	}
	return json_encode(array("returnCode" => '0', 'message' => "logout incomplete something went wrong")); //returns if something went wrong.
}	
?>
