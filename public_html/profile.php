<?php
require_once(rabbitFunctions.php);

$services = array("type"=>'get_service_list', "username"=>$_COOKIE["username"]);
$servresponse = json_decode(sendDB($services), true);
$servarr = $servresponse["serviceArray"];
$requestService = array("type"=>'get_service_titles',"services"=>$servarr);
$services = json_decode(sendDB($requestService), true);
$servarr = $services["services"];


$movies = array("type"=>'get_movie_list', "username"=>$_COOKIE["username"]);
$movresponse = json_decode(sendDB($movies), true);
$movarr = $movresponse["movieArray"];
$requestMovie = array("type"=>'get_movie_titles', "movies"=>$movarr);
$movies = json_decode(sendDB($requestMovie), true);
$movarr = $movies["movies"];



$display_block .= "<th><strong>Services</strong></th><tr><td><strong>$servarr</strong></td></tr><th><strong>Movies</strong></th><tr><td><strong>$movarr</strong></td></tr>";

?>


<html>
<head>
Words
<?php echo "Username: ".$_COOKIE["username"]); ?>
</head>
<body>
<?php echo $display_block; ?>
</body>
</html>
