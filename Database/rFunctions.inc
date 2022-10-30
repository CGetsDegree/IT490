<?php

require_once('Connection.php');

function returnUser($username, $db) {
	$query = "select * from users where username = '$username'";
	$result = $db->query($query);
	while ($row = $result->fetch_assoc()) {
		if($row["username"] == $username) {
			return $row['id'];
		}
	}
	return null;
}
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
					return json_encode(array("returnCode" => '1', 'message'=>"Passwords Match", 'sessionId'=>'$sessionid')); // password matched with username, allows login
				}
				else {
					return json_encode(array("returnCode" => '0', 'message'=>"Error generating session")); //session wasn't generated correctly. Error
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
	}
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
		if ($logindb->query($delete) === TRUE) { //checks to make sure it worked
			return json_encode(array("returnCode" => '1', 'message' => "Logout successfull")); //returns if logout worked 
		} 
		return json_encode(array("returnCode" => '0', 'message' => "already logged out")); //returns when user is already logged out
	}
	
	}
	return json_encode(array("returnCode" => '0', 'message' => "logout incomplete something went wrong")); //returns if something went wrong.
}	

/*
* Adds a service to the users list of services
*/
function addService($userid, $serviceid) {
	$logindb = databaseConnect();
	$usid = returnUser($userid, $logindb);
	if ($usid == NULL) {
		return json_encode(array("returnCode" => '0', 'message' => "problem getting user id"));
	}
	$uid = $logindb->real_escape_string($usid);
	$sid = $logindb->real_escape_string($serviceid);
	$query = "select * from service_list where id = '$uid' and serviceID = '$sid'";
	$select = $logindb->query($query);
	while ($row = $select->fetch_assoc()) {
		if($row["id"] == $uid && $row["serviceID"] == $sid) {
			return json_encode(array("returnCode" => '0', 'message' => "service already added"));
		}
	}
	$insert = "insert into service_list values('$uid', '$sid')";
	if ($logindb->query($insert) === TRUE) {
		return json_encode(array("returnCode" => '1', 'message' => "service successfully added"));
		
	}
	return json_encode(array("returnCode" => '0', 'message' => "no service added. Something went wrong"));
}

/*
* removes a service to the users list of services
*/
function removeService($userid, $serviceid) {
	$logindb = databaseConnect();
	if ($usid == NULL) {
		return json_encode(array("returnCode" => '0', 'message' => "problem getting user id"));
	}
	$uid = $logindb->real_escape_string($usid);
	$sid = $logindb->real_escape_string($serviceid);
	$query = "select * from service_list where id = '$uid' and serviceID = '$sid'";
	$select = $logindb->query($query);
	while ($row = $select->fetch_assoc()) {
		if($row["id"] == $uid && $row["serviceID"] == $sid) {
			$insert = "insert into service_list values('$uid', '$sid')";
			if ($logindb->query($insert) === TRUE) {
				return json_encode(array("returnCode" => '1', 'message' => "service successfully removed"));			
			}
		}
	}
		
	return json_encode(array("returnCode" => '0', 'message' => "no service removed. Something went wrong"));
}

/*
* pulls a list of every service the user has
*/
function getServices($userid) {
	$sarray = array();
	$logindb = databaseConnect();
	if ($usid == NULL) {
		return json_encode(array("returnCode" => '0', 'message' => "problem getting user id"));
	}
	$uid = $logindb->real_escape_string($usid);
	$query = "select * from service_list where id = '$uid'";
	$select = $logindb->query($query);
	while ($row = $select->fetch_assoc()) {
		if($row["id"] == $uid) {
			array_push($sarray, $row["serviceID"]);
		}
	}
	return json_encode(array("returnCode" => '1', 'message' => "service list made successful", 'serviceArray'=>$sarray)); //returns an array of movies in the list.
}

/*
* Adds a movie to the users list of services
*/
function addMovie($userid, $movieid) {
	$logindb = databaseConnect();
	if ($usid == NULL) {
		return json_encode(array("returnCode" => '0', 'message' => "problem getting user id"));
	}
	$uid = $logindb->real_escape_string($usid);
	$mid = $logindb->real_escape_string($movieid);
	$query = "select * from watch_list where id = '$uid' and movieID = '$mid'";
	$select = $logindb->query($query);
	while ($row = $select->fetch_assoc()) {
		if($row["id"] == $uid && $row["movieID"] == $mid) {
			return json_encode(array("returnCode" => '0', 'message' => "service already added"));
		}
	}
	$insert = "insert into watch_list values('$uid', '$mid')";
	if ($logindb->query($insert) === TRUE) {
		return json_encode(array("returnCode" => '1', 'message' => "service successfully added"));
		
	}
	return json_encode(array("returnCode" => '0', 'message' => "no service added. Something went wrong"));
}

/*
* removes a movie to the users list of services
*/
function removeMovie($userid, $movieid) {
	$logindb = databaseConnect();
	if ($usid == NULL) {
		return json_encode(array("returnCode" => '0', 'message' => "problem getting user id"));
	}
	$uid = $logindb->real_escape_string($usid);
	$mid = $logindb->real_escape_string($serviceid);
	$query = "select * from watch_list where id = '$uid' and movieID = '$mid'";
	$select = $logindb->query($query);
	while ($row = $select->fetch_assoc()) {
		if($row["id"] == $uid && $row["movieID"] == $mid) {
			$insert = "insert into watch_list values('$uid', '$mid')";
			if ($logindb->query($insert) === TRUE) {
				return json_encode(array("returnCode" => '1', 'message' => "movie successfully removed"));			
			}
		}
	}
		
	return json_encode(array("returnCode" => '0', 'message' => "no movie removed. Something went wrong"));
}

/*
* pulls a list of every service the user has
*/
function getMovies($userid) {
	$sarray = array();
	$logindb = databaseConnect();
	if ($usid == NULL) {
		return json_encode(array("returnCode" => '0', 'message' => "problem getting user id"));
	}
	$uid = $logindb->real_escape_string($usid);
	$query = "select * from watch_list where id = '$uid'";
	$select = $logindb->query($query);
	while ($row = $select->fetch_assoc()) {
		if($row["id"] == $uid) {
			array_push($sarray, $row["movieID"]);
		}
	}
	return json_encode(array("returnCode" => '1', 'message' => "movie list made successful", 'serviceArray'=>$sarray)); //returns an array of movies in the list.
}

/*
* Adds a rating on request
*/
function addRating($userid, $movieid, $rating, $db) {
	$query = "insert into ratings values('$userid', '$movieid', '$rating', '$db')";
	if ($db->query($query) === TRUE) {
		return 1;
	}
	return 0;
}

/*
* Changes the rating on request or adds it if it didn't exist
*/
function changeRating($userid, $movieid, $rating) {
	$logindb = databaseConnect();
	if ($usid == NULL) {
		return json_encode(array("returnCode" => '0', 'message' => "problem getting user id"));
	}
	$uid = $logindb->real_escape_string($usid);
	$mid = $logindb->real_escape_string($movieid);
	$rate = $logindb->real_escape_string($rating);
	$query = "select * from ratings where id = '$uid' and movieID = '$mid' and ratings = '$rate'";
	$select = $logindb->query($query);
	while ($row = $select->fetch_assoc()) {
		if($row["id"] == $uid && $row["movieID"] == $mid && $row["ratings"] == $rate) {
			return json_encode(array("returnCode" => '0', 'message' => "rating is already that rating"));
		}
		else if($row["id"] == $uid && $row["movieID"] == $mid && $row["ratings"] != $rate) {
			$change = "UPDATE ratings set ratings = '$rate' where id = '$uid' and movieID = '$mid'";
			if($logindb->query($change) === TRUE) {
				return json_encode(array("returnCode" => '1', 'message' => "rating update successful"));
			}
		}
	}
	if (addRating($uid, $mid, $rate, $logindb)) {
		return json_encode(array("returnCode" => '1', 'message' => "successfully changed the rating"));
	}
		
	return json_encode(array("returnCode" => '0', 'message' => "rating not changed"));
}

function getRating($userid, $movieid) {
	$sarray = array();
	$logindb = databaseConnect();
	if ($usid == NULL) {
		return json_encode(array("returnCode" => '0', 'message' => "problem getting user id"));
	}
	$uid = $logindb->real_escape_string($usid);
	$mid = $logindb->real_escape_string($movieid);
	$query = "select * from ratings where id = '$uid' and movieID = '$mid'";
	$select = $logindb->query($query);
	while ($row = $select->fetch_assoc()) {
		if($row["id"] == $uid && $row["movieID"] == $mid) {
		return json_encode(array("returnCode" => '1', 'message' => "rating achieved", 'rating'=>$row["rating"])); //returns the rating for the movie
		}
	}
	return json_encode(array("returnCode" => '0', 'message' => "failed to get a rating", 'serviceArray'=>$sarray)); 
}
?>
