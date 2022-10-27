<?php

require_once('Connection.php');

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
				return array("returnCode" => '0', 'message'=>"Username already exist try again");// password match
			}
		}
		$insert = "instert into users values(0, ".'$un'.",". '$pw'.")";
		if ($logindb->query($insert) === TRUE) {
			return array("returnCode" => '1', 'message'=>"Username and password accepted");
		}
		return array("returnCode" => '0', 'message'=>"something went wrong try again"); 
	}
?>
