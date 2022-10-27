<?php
class loginDB
{
private $logindb;

public function __construct()
	{
		$this->logindb = new mysqli("127.0.0.1","kepsin","12345","IT490");

		if ($this->logindb->connect_errno != 0)
		{
			echo "Error connecting to database: ".$this->logindb->connect_error.PHP_EOL;
			exit(1);
		}
		echo "correctly connected to database".PHP_EOL;
	}

public function doLogin($username,$password)
	{
		$un = $this->logindb->real_escape_string($username);
		$pw = $this->logindb->real_escape_string($password);
		$statement = "select * from users where username = '$un'";
		$response = $this->logindb->query($statement);

		while ($row = $response->fetch_assoc())
		{
			echo "checking password for $username".PHP_EOL;
			if ($row["password"] == $pw)
			{
				echo "passwords match for $username".PHP_EOL;
				return array("returnCode" => '1', 'message'=>"Passwords Match");// password match
			}
			echo "passwords did not match for $username".PHP_EOL;
		}
		return array("returnCode" => '0', 'message'=>"No Users Match Username");//no users matched username
	}
}
?>
