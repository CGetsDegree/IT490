<?php

error_reporting(E_ERROR | E_WARNING | E_PARSE | E_NOTICE);
ini_set('display_errors', 1);

//Access to RMQ
include("djmvClient.php");
include("vmdajClient.php");

//registration.html
$username = $_POST["Username"];
$password = $_POST["Password"];
$name = $_POST["Name"];
$email = $_POST["Email"];

        //Session incase user,password,type needed elsewhere 
        $_SESSION["username"] = $username;
        $_SESSION["password"] = $password;
        $_SESSION["message"] = "Newby";
	//RMQ message to DB Queue
	header("refresh:1; url = 'djmvClient.php'");

//MySQL access
$host = "localhost";
$user = "mk694";
$pass = "testmk694";
$dbname = "userlogin";

$con = mysqli_connect($host, $user, $pass, $dbname);


        $newUser = mysqli_query($con, "INSERT INTO user(Username, Password, Name, Email) VALUES('$username', '$password', '$name', '$email')");

        if($con->query($newUser) == FALSE) { $_SESSION["message"]= "New Member";
				//RMQ message to API Queue
                                header("refresh:1; url = 'vmdajClient.php'");
				//Redirect link to API page
                                header("refresh:1; url = 'http://api2.bigoven.com'");  }

	else {  $_SESSION["message"]= "Creation issue";
	      	//RMQ message to DB Queue
                header("refresh:1; url = 'djmvClient.php'");
	      	//Redirect back to registration page
                $url = "register.html";
                header ("refresh:2; url =$url");  }


?>
