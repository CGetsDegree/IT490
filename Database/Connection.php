<?php
function databaseConnect() {
		$logindb = new mysqli("127.0.0.1","kepsin","12345","IT490");

		if ($logindb->connect_errno != 0)
		{
			echo "Error connecting to database: ".$this->logindb->connect_error.PHP_EOL;
			exit(1);
		}
		echo "correctly connected to database".PHP_EOL;
		return $logindb;
		}

?>
