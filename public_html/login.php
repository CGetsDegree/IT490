<!DOCTYPE html>
<html>
    <body>
        <div id="failure"<?php
        if (!empty($_POST["message"])){
                if ($_POST["message"] == "BadCredentials"){
                        echo "";
		}
		else{
			echo "hidden";
		}
	}
	else{
		echo"hidden";
	}
?>><h1>Failed to log in.</h1></div>
        <div id="loginform">
            <form id="loginForm" action="home.php" method="post">
                <h4>Log In</h4>
                <table style="text-align: left;">
                    <tr><th>Username:</th><td><input type="text" id="username" name="username" size="16" required></td></tr>
                    <tr><th>Password:</th><td><input type="password" id="password" name="password" size="16" required></td></tr>
                </table>
                <input name="submit" type="submit" value="Submit">
            </form>
        </div>
    </body>
</html>

<style>
#failure{
  border-radius:20px;
  padding:2px;
  padding-left:10px;
  background: brown;
}

#login{
  border-radius:20px;
  padding:2px;
  padding-left:10px;
  background: steelblue;
}

input{
  background: silver;
}

body {
  font-family: Helvetica, Verdana, sans-serif;
  background: lightslategray;
}

</style>
