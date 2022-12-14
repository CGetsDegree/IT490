<?php

require_once('Connection.php');
require_once('../rabbitFunctions.php');

/*
* returns the userid from the list of users.
*/
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
					sendLog('Login: Login successful');
					var_dump(json_encode(array("returnCode" => '1', 'message'=>"Passwords Match", 'sessionid'=>$sessionid)));
					return json_encode(array("returnCode" => '1', 'message'=>"Look at that you logged in lucky dog", 'sessionid'=>$sessionid)); // password matched with username, allows login
				}
				else {
					sendLog('Login: Error generating session');
					return json_encode(array("returnCode" => '0', 'message'=>"Error generating session")); //session wasn't generated correctly. Error
				}
				
			}
			sendLog('Login: Passwords do not match');
			echo "passwords did not match for $username".PHP_EOL; //passwords didn't match, continues through list of users.
		}
		sendLog('Login: Usernames do not match');
		return json_encode(array("returnCode" => '0', 'message'=>"No Users Match Username")); //no users matched username, no login
	}
	
/*
* Creates a new profile and checks to see if username is taken already.
* Puts the profile in the table users.
*/		
function validateRegister($username,$password, $email)
	{
		$fcchecker = false;
		$logindb = databaseConnect();
		$un = $logindb->real_escape_string($username);
		$em = $logindb->real_escape_string($email);
		$pw = $logindb->real_escape_string($password);
		$fc = $logindb->real_escape_string(rand(100000, 99999999));
		$statement = "select * from users where username = '$un' AND email = '$em'";
		$response = $logindb->query($statement);

		while ($row = $response->fetch_assoc())
		{
			echo "checking usernam for $username".PHP_EOL;
			if ($row["username"] == $un)
			{
				echo "username already exist".PHP_EOL;
				sendLog('Register: Username is already in use');
				return json_encode(array("returnCode" => '0', 'message'=>"Username already exist try again")); //username is already being used 
			}
			if ($row["email"] == $em)
			{
				sendLog('Register: Email is already in use');
				return json_encode(array("returnCode" => '0', 'message'=>"Email already exist try again"));
			}
		}
		while ($fcchecker == false) {
			while ($row = $response->fetch_assoc()) 
			{
				if ($row["friend_code"] == $fc)
				{
					$fc = $logindb->real_escape_string(rand(100000, 99999999));
					break;
				}
			}
			$fcchecker = true;
		}
		$insert = "insert into users values(0, '$un', '$pw', '$fc', '$em')"; //creates a new user and password in users table
		if ($logindb->query($insert) === TRUE) { //check to make sure it worked
			sendLog('Register: Username and passwords accepted');
			return json_encode(array("returnCode" => '1', 'message'=>"Username and password accepted")); //returns that it worked to user
		}
		sendLog('Register: Error parsing thorugh if statements');
		return json_encode(array("returnCode" => '0', 'message'=>"something went wrong try again")); //something went wrong in the registration of the user.
	}
/*
* Creates a sessionid for the user when logged in. 
* Puts the sessionid in sessions table with expiration time and time of creation.
*/
function createSession($sessionId) {
	$logindb = databaseConnect();
	$session = $logindb->real_escape_string($sessionId);
	$query = "select * from sessions where sessionID = '$session'";
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
	$query = "select * from sessions where sessionID = '$session' AND expireTime > current_time()"; //selects all current sessions that aren't expired
	$response = $logindb->query($query);
	while ($row = $response->fetch_assoc()) {
		if ($row["sessionID"] == $session) {
			sendLog('Sessions: ID is valid, user logged in');
			return json_encode(array("returnCode" => '1', 'message' => "SessionID is valid")); //returns a valid sessionID and allows the user to stay logged in.
		} 
	}
	stopSession($sessionId); //stops the session if it no longer is valid.
	sendLog('Sessions: ID is invalid');
	return json_encode(array("returnCode" => '0', 'message' => "SessionID is invalid"));
}

/*
* Allows the user to logout and deletes the sessionID from sessions table.
*/
function stopSession($sessionId) {
	$logindb = databaseConnect();
	$session = $logindb->real_escape_string($sessionId);
	$query = "select * from sessions where sessionID = '$session'";
	$response = $logindb->query($query);
	while ($row = $response->fetch_assoc()) {
		if ($row["sessionID"] == $session) {
			$delete = "delete from sessions where sessionID='$session'"; //deletes the session from the table sessions
		if ($logindb->query($delete) === TRUE) { //checks to make sure it worked
			sendLog('Sessions: User logged out');
			return json_encode(array("returnCode" => '1', 'message' => "Logout successfull")); //returns if logout worked 
		} 
		sendLog('Sessions: User alread logged out');
		return json_encode(array("returnCode" => '0', 'message' => "already logged out")); //returns when user is already logged out
	}
	
	}
	sendLog('Sessions: Logging out error');
	return json_encode(array("returnCode" => '0', 'message' => "logout incomplete something went wrong")); //returns if something went wrong.
}	

/*
* Adds a service to the users list of services
*/
function addService($userid, $serviceid) {
	$logindb = databaseConnect();
	$usid = returnUser($userid, $logindb);
	if ($usid == NULL) {
		sendLog('DB: Problem getting user ID');
		return json_encode(array("returnCode" => '0', 'message' => "problem getting user id"));
	}
	$uid = $logindb->real_escape_string($usid);
	$sid = $logindb->real_escape_string($serviceid);
	$query = "select * from service_list where id = '$uid' and serviceID = '$sid'";
	$select = $logindb->query($query);
	while ($row = $select->fetch_assoc()) {
		if($row["id"] == $uid && $row["serviceID"] == $sid) {
			sendLog('DB: Service already added');
			return json_encode(array("returnCode" => '0', 'message' => "service already added"));
		}
	}
	$insert = "insert into service_list values('$uid', '$sid')";
	if ($logindb->query($insert) === TRUE) {
		sendLog('DB: Service succesfully added');
		return json_encode(array("returnCode" => '1', 'message' => "service successfully added"));
		
	}
	sendLog('DB: No service added, something went wrong');
	return json_encode(array("returnCode" => '0', 'message' => "no service added. Something went wrong"));
}

/*
* removes a service to the users list of services
*/
function removeService($userid, $serviceid) {
	$logindb = databaseConnect();
	$usid = returnUser($userid, $logindb);
	if ($usid == NULL) {
		sendLog('DB: Problem getting userID');
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
				sendLog('DB: Service successfully removed');
				return json_encode(array("returnCode" => '1', 'message' => "service successfully removed"));			
			}
		}
	}
	sendLog('DB: No service removed, something went wrong');	
	return json_encode(array("returnCode" => '0', 'message' => "no service removed. Something went wrong"));
}

/*
* pulls a list of every service the user has
*/
function getServices($userid) {
	$sarray = array();
	$logindb = databaseConnect();
	$usid = returnUser($userid, $logindb);
	if ($usid == NULL) {
		sendLog('DB: Problem getting userID');
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
	sendLog('DB: Service list created successful');
	return json_encode(array("returnCode" => '1', 'message' => "service list made successful", 'serviceArray'=>$sarray)); //returns an array of movies in the list.
}

/*
* Adds a movie to the users list of services
*/
function addMovie($userid, $movieid, $movieTitle) {
	$logindb = databaseConnect();
	$usid = returnUser($userid, $logindb);
	if ($usid == NULL) {
		sendLog('DB: Problem getting userID');
		return json_encode(array("returnCode" => '0', 'message' => "problem getting user id"));
	}
	$uid = $logindb->real_escape_string($usid);
	$mid = $logindb->real_escape_string($movieid);
	$mt = $logindb->real_escape_string($movieTitle);
	$query = "select * from watch_list where id = '$uid' and movieID = '$mid'";
	$select = $logindb->query($query);
	while ($row = $select->fetch_assoc()) {
		if($row["id"] == $uid && $row["movieID"] == $mid) {	
			sendLog('DB: Service already added');
			return json_encode(array("returnCode" => '0', 'message' => "service already added"));
		}
	}
	$insert = "insert into watch_list values('$uid', '$mid', '$mt')";
	if ($logindb->query($insert) === TRUE) {
		sendLog('DB: Service successfully added');
		return json_encode(array("returnCode" => '1', 'message' => "service successfully added"));
		
	}
	sendLog('DB: No service added, something went wrong');
	return json_encode(array("returnCode" => '0', 'message' => "no service added. Something went wrong"));
}

/*
* removes a movie to the users list of services
*/
function removeMovie($userid, $movieid) {
	$logindb = databaseConnect();
	$usid = returnUser($userid, $logindb);
	if ($usid == NULL) {
		sendLog('DB: Problem getting userID');
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
				sendLog('Movie successfully removed');
				return json_encode(array("returnCode" => '1', 'message' => "movie successfully removed"));			
			}
		}
	}
	sendLog('No movie removed, soemthing went wrong');
	return json_encode(array("returnCode" => '0', 'message' => "no movie removed. Something went wrong"));
}

/*
* pulls a list of every service the user has
*/
function getMovies($userid) {
	$sarray = array();
	$logindb = databaseConnect();
	$usid = returnUser($userid, $logindb);
	if ($usid == NULL) {
		sendLog('Problem getting userID');
		return json_encode(array("returnCode" => '0', 'message' => "problem getting user id"));
	}
	$uid = $logindb->real_escape_string($usid);
	$query = "select * from watch_list where id = '$uid'";
	$select = $logindb->query($query);
	while ($row = $select->fetch_assoc()) {
		if($row["id"] == $uid) {
			array_push($sarray, array("title"=>$row["movieName"], "id"=>$row['movieid']));
		}
	}
	sendLog('Movie list generated successful');
	return json_encode(array("returnCode" => '1', 'message' => "movie list made successful", 'movieArray'=>$sarray)); //returns an array of movies in the list.
}

/*
* Adds a rating on request
*/
function addRating($userid, $movieid, $rating, $db) {
	$query = "insert into ratings values('$userid', '$movieid', '$rating')";
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
	$usid = returnUser($userid, $logindb);
	if ($usid == NULL) {
		sendLog('Problem getting userID');
		return json_encode(array("returnCode" => '0', 'message' => "problem getting user id"));
	}
	$uid = $logindb->real_escape_string($usid);
	$mid = $logindb->real_escape_string($movieid);
	$rate = $logindb->real_escape_string($rating);
	$query = "select * from ratings where id = '$uid' and movieID = '$mid'";
	$select = $logindb->query($query);
	while ($row = $select->fetch_assoc()) {
		echo "entered the while loop".PHP_EOL;
		if($row["id"] == $uid && $row["movieID"] == $mid && $row["ratings"] == $rate) {
			sendLog('Rating error, already that rating');
			echo "rating hasn't changed".PHP_EOL;
			return json_encode(array("returnCode" => '1', 'message' => "rating is already that rating", "rating"=>$rating));
		}
		echo $rate.PHP_EOL;
		echo $row["ratings"].PHP_EOL;
		if($row["id"] == $uid && $row["movieID"] == $mid && $row["ratings"] != $rate) {		
			echo "Preparing to update rating".PHP_EOL;
			$change = "UPDATE ratings set ratings = '$rate' where id = '$uid' and movieID = '$mid'";
			if($logindb->query($change) === TRUE) {
				sendLog('Rating updated successfully');
				return json_encode(array("returnCode" => '1', 'message' => "rating update successful", "rating"=>$rating));
			}
		}
	}
	echo "attempting to add rating".PHP_EOL;
	if (addRating($uid, $mid, $rate, $logindb)) {
		sendLog('Changed rating successful');
		return json_encode(array("returnCode" => '1', 'message' => "successfully changed the rating", "rating"=>$rating));
	}
	sendLog('Rating not changed correctly');
	return json_encode(array("returnCode" => '0', 'message' => "rating not changed"));
}

/*
* Gets the rating and gives it to the webserver
*/
function getRating($userid, $movieid) {
	$sarray = array();
	$logindb = databaseConnect();
	if ($usid == NULL) {
		sendLog('Problem getting userID');
		return json_encode(array("returnCode" => '0', 'message' => "problem getting user id"));
	}
	$uid = $logindb->real_escape_string($usid);
	$mid = $logindb->real_escape_string($movieid);
	$query = "select * from ratings where id = '$uid' and movieID = '$mid'";
	$select = $logindb->query($query);
	while ($row = $select->fetch_assoc()) {
		if($row["id"] == $uid && $row["movieID"] == $mid) {
		sendLog('Returning rating of movie');
		return json_encode(array("returnCode" => '1', 'message' => "rating achieved", 'rating'=>$row["rating"])); //returns the rating for the movie
		}
	}
	sendLog('Failed to return rating');
	return json_encode(array("returnCode" => '0', 'message' => "failed to get a rating", 'serviceArray'=>$sarray)); 
}

/* 
* Returns the users info for profile
*/
function getUserInfo($username) {
	$logindb = databaseConnect();
	$uid = $logindb->real_escape_string($username);
	$query = "select friend_code, email from users where username = '$uid'";
	$select = $logindb->query($query);
	while ($row = $select->fetch_assoc()) {
		return json_encode(array("returnCode" => '1', 'message' => "profile returned", 'friend_code'=>$row["friend_code"], 'email'=>$row['email'])); //returns the rating for the movie
		}
	sendLog('Failed to return profile');
	return json_encode(array("returnCode" => '0', 'message' => "failed to get profile")); 
}

function addFriend($username, $friendcode) {
	$fid;
	$logindb = databaseConnect();
	$usid = returnUser($username, $logindb);
	if ($usid == NULL) {
		sendLog('Problem getting userID');
		return json_encode(array("returnCode" => '0', 'message' => "problem getting user id"));
	}
	$fc = $logindb->real_escape_string($friendcode);
	$query = "select username from users where friend_code = '$fc'";
	$select = $logindb->query($query);
	while ($row = $select->fetch_assoc()) {
		$fid = $row['username'];
	}
	$insert = "insert into friends_list values('$usid', '$fid')";
	if ($logindb->query($insert) === TRUE) {
		sendLog('Added to Friendslist');
		return json_encode(array("returnCode" => '1', 'message' => "successful addition"));
	}
	sendLog('Failed to send to friendlist');
	return json_encode(array("returnCode" => '0', 'message' => "problem adding friend"));
}
function getFriends($username) {
	$sarray = array();
	$logindb = databaseConnect();
	$usid = returnUser($username, $logindb);
	if ($usid == NULL) {
		sendLog('Problem getting userID');
		echo "failure";
		return json_encode(array("returnCode" => '0', 'message' => "problem getting user id"));
	}
	$query = "select * from friends_list where userid = '$usid'";
	$select = $logindb->query($query);
	while($row = $select->fetch_assoc()) {
		array_push($sarray, $row['frienduser']);
	}
	sendlog("friendlist sent");
	echo json_encode(array("returnCode" => '1', 'message' => "friends list sent and made", 'friendsArray'=>$sarray));
	return json_encode(array("returnCode" => '1', 'message' => "friends list sent and made", 'friendsArray'=>$sarray));
	
}
	
?>
