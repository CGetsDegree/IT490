<?php
require_once("rabbitFunctions.php");
$movieID = $_GET["id"];
$request = array("type"=>"get_movie_details", "id"=>$movieID);
$response = sendDB($request);
$response = json_decode($response, true);
$display_block = "<table>";
$title = $response["title"];
$runtime=$response["runtime"];
$prod_company = $response["production_companies"][0]["name"];
$overview = $response["overview"];
$release_date = $response["release_date"];
$genre=array();
foreach($response["genres"] as $g){
	$genre[]=$g["name"];
}
$genres = implode(", ",$genre);
$stream_services=array();
foreach($response["streaming_services"] as $key=>$val){
	$stream_services[]=$val;
}
$streaming = implode("</td><td>",$stream_services);
$cast="";
foreach($response["cast"] as $key=>$val){
	$cast.="<tr><td>$key</td><td>$val</td></tr>";
}
$director = $response["director"];
$producer = $response["producers"][0];
$writer = $response["writers"][0];
	$display_block .= "
		<th>
		<strong>$title</strong>
		</th>
		<tr><td rowspan='2' colspan='3'><strong>$overview</strong></td><td><strong>$release_date</strong></td></tr>
		<tr><td><strong>$prod_company</strong></td></tr>
		<tr><td><strong>Genres</strong></td><td>$genres</td><td><strong>Runtime</strong></td><td>$runtime  min</td></tr>
		</table>
		<br>
		<table>
		<tr><td>Director</td><td>Producer</td><td>Writer</td></tr>
		<tr><td><strong>$director</strong></td><td><strong>$producer</strong></td><td><strong>$writer</strong></td></tr>";
$display_block .= "</table><br>";
$display_block.="<table><th><strong>Cast</strong></th>$cast</table><br>";
$display_block.="<table><th><strong>Streaming on</strong></th><tr><td>$streaming</td></tr></table><br>";
?>

<html>
<head>
<title>Movie Details</title>
</head>
<body>
<?php echo $display_block; ?>
<table><th>Rate this movie</th>
<tr><select name="rating" id="rating" onchange="rate()" onfocus="this.selectedIndex=-1;">
<option value=1>1</option>
<option value=2>2</option>
<option value=3>3</option>
<option value=4>4</option>
<option value=5>5</option>
</select>
</tr>
<tr>
<button name="addto" id="addto" onclick="addToWatchList()">Add to watchlist</button>
</tr>
</table>
</body>
</html>

<style>
table, tr{
border:2px solid black
}
td{
border: 1px solid black
}
</style>
<script type="text/javascript">
async function rate(){
	let sel=document.querySelector("#rating");
	const sessionid="<?php echo $_COOKIE["sessionid"]?>";
	const username="<?php echo $_COOKIE["username"]?>";
	const movie_id=<?php echo $_GET["id"]?>;
	let fd=new FormData();
	fd.append("username",username);
	fd.append("type","change_movie_rating");
	fd.append("rating",sel.value);
	fd.append("movie_id",movie_id);
	console.log(sel.value);
	const result=await send_rating(fd);
	console.log(result);
	if (result.returnCode=="1"){
		let good=document.createElement("td");
		good.innerText="Rating sent successfully!";
		sel.parentNode.replaceChild(good,sel);
	}
	else{
                let good=document.createElement("td");
                good.innerText="Rating could not be saved!";
                sel.parentNode.replaceChild(good,sel);
        }
}
async function addToWatchlist(){
        const sessionid="<?php echo $_COOKIE["sessionid"]?>";
        const username="<?php echo $_COOKIE["username"]?>";
        const movie_id=<?php echo $_GET["id"]?>;
        let fd=new FormData();
        fd.append("username",username);
        fd.append("type","add_movie_list");
        fd.append("sessionid",sessionid);
        fd.append("movie_id",movie_id);
        const result=await send_rating(fd);
        console.log(result);
}
async function send_rating(fd){
	try{
	const res=await fetch("movies.php",{
	method:"POST",
	body:fd});
	return res.data;
	}
	catch(error){
		return error;
	}
}
</script>
