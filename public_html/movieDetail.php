<?php
require_once("rabbitFunctions.php");
$request = array("type"=>"get_movie_details", "id"=>11);
$response = sendDB($request);
var_dump($response);
$response = json_decode($response, true);
var_dump($response);
$movieID = $post["id"];
$display_block = "<table>";
$title = $response["title"];
$prod_company = $response["production_companies"];
$overview = $response["overview"];
$release_date = $response["release_date"];
$genres = $response["genres"];
$streaming = $response["streaming_services"];
$cast = $response["cast"];
$director = $response["director"];
$producer = $response["producer"][0];
$writer = $response["writer"];
foreach($prod_company as $item){
	$display_block .= "<tr><td><strong>$prod_company</strong><br></td></tr>";
}
	$display_block .= "
		<tr>
		<td>
		<strong>$title</strong><br>

		<strong>$overview</strong><br>
		<strong>$release_date</strong><br>
		<strong>$genres</strong><br>
		<strong>$streaming</strong><br>
		<strong>$cast</strong><br>
		<strong>$director</strong><br>
		<strong>$producer</strong><br>
		<strong>$writer</strong><br>
		</td>
		</tr>";
$display_block .= "</table>";
?>


<html>
<head>
<title>Movie Details</title>
</head>
<body>
<?php print $display_block; ?>
</body>
</html>
