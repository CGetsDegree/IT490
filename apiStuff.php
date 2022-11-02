<?php
//$url = "https://api.themoviedb.org/3/search/movie?api_key=$API_KEY&language=en-US&query=Fight%20Club";

//$response = getServices(11);
//$output = array();
//searchMovies("Rogue One");
//$movies = array(11, 436270, 550, 1919);
//displayWatchlist($movies);
//echo $result;
//$output = json_decode($response, true);

//print_r($response);

require_once('rabbitFunctions.php');

function print_genres($arr){
	foreach($arr as $items){
		echo $items['name'] . ", ";
	}
}

function getServices($id){
	$API_KEY = "f94eca9d744133b72f98995bdc4cca12";
	$url = "https://api.themoviedb.org/3/movie/$id/watch/providers?api_key=$API_KEY";
	$options = array(
		CURLOPT_RETURNTRANSFER => true,
		//CURLOPT_HEADER => false,
		CURLOPT_FOLLOWLOCATION => true,
		CURLOPT_HTTPHEADER => array("Accept:application/json"));
	$get_services = curl_init($url);
	curl_setopt_array($get_services, $options);
	$result = curl_exec($get_services);
	curl_close($get_services);
	$result = json_decode($result,true);
	return $result["results"]["US"];
}
function getMovie($id){
	$API_KEY = "f94eca9d744133b72f98995bdc4cca12";
	$url = "https://api.themoviedb.org/3/movie/$id?api_key=$API_KEY&language=en-US";
	$options = array(
		CURLOPT_RETURNTRANSFER => true,
		//CURLOPT_HEADER => false,
		CURLOPT_FOLLOWLOCATION => true,
		CURLOPT_HTTPHEADER => array("Accept:application/json"));

	$get_movie = curl_init($url);
	curl_setopt_array($get_movie, $options);
	$resultOne = curl_exec($get_movie);
	curl_close($get_movie);
	$resultOne = json_decode($resultOne,true);
	//print_r($resultOne);
	return $resultOne;
}
function getMovieDetails($id){
	$API_KEY = "f94eca9d744133b72f98995bdc4cca12";
	$url = "https://api.themoviedb.org/3/movie/$id?api_key=$API_KEY&language=en-US";
	$options = array(
		CURLOPT_RETURNTRANSFER => true,
		//CURLOPT_HEADER => false,
		CURLOPT_FOLLOWLOCATION => true,
		CURLOPT_HTTPHEADER => array("Accept:application/json"));

	$get_movie = curl_init($url);
	curl_setopt_array($get_movie, $options);
	$resultOne = curl_exec($get_movie);
	curl_close($get_movie);
	$resultOne = json_decode($resultOne,true);
	$resultTwo = getServices($id);
	$resultThree = getCredits($id);
	//print_r($resultOne);
	//print_r($resultTwo);
	//print_r($resultThree);
	$result = array(
		"id"=>$resultOne["id"], 
		"title"=>$resultOne["title"],
		"production_companies"=>$resultOne["production_companies"],
		"poster_path"=>$resultOne["poster_path"],
		"overview"=>$resultOne["overview"],
		"release_date"=>$resultOne["release_date"],
		"genres"=>$resultOne["genres"]);
	$services = array();
	foreach($resultTwo["flatrate"] as $service){
		$services[$service["provider_id"]] = $service["provider_name"];
	}
	$result["streaming_services"] = $services;
	$actors = array();
	for($i = 0; $i < 10; $i++){
		$actors[$resultThree["cast"][$i]["name"]] = $resultThree["cast"][$i]["character"];
	}
	$result["cast"] = $actors;
	$result["producers"] = array();
	$result["writers"] = array();
	foreach($resultThree["crew"] as $person){
		if($person["job"] == "Director"){
			$result["director"] = $person["name"];
		}
		if($person["job"] == "Executive Producer"){
			$result["producers"][] = $person["name"];
		}
		if($person["job"] == "Writer" or $person["job"] == "Screenplay"){
			$result["writers"][] = $person["name"];
		}
	}
		
	//print_r($result);
	return $result;
}

function getCredits($id){
	$API_KEY = "f94eca9d744133b72f98995bdc4cca12";
	$url = "https://api.themoviedb.org/3/movie/$id/credits?api_key=$API_KEY&language=en-US";
	$options = array(
		CURLOPT_RETURNTRANSFER => true,
		//CURLOPT_HEADER => false,
		CURLOPT_FOLLOWLOCATION => true,
		CURLOPT_HTTPHEADER => array("Accept:application/json"));

	$get_credits = curl_init($url);
	curl_setopt_array($get_credits, $options);
	$result = curl_exec($get_credits);
	curl_close($get_credits);
	return json_decode($result, true);
}
function searchMovies($query, $page=1){
	$API_KEY = "f94eca9d744133b72f98995bdc4cca12";
	$url = "https://api.themoviedb.org/3/search/movie?api_key=$API_KEY&language=en-US&query=" . urlencode($query) . "&page=$page";
	$options = array(
		CURLOPT_RETURNTRANSFER => true,
		//CURLOPT_HEADER => false,
		CURLOPT_FOLLOWLOCATION => true,
		CURLOPT_HTTPHEADER => array("Accept:application/json"));
	$searchMovies = curl_init($url);
	curl_setopt_array($searchMovies, $options);
	$result = curl_exec($searchMovies);
	curl_close($searchMovies);
	$result = json_decode($result,true);
	$info = array();
	foreach($result["results"] as $movie){
		$details = getMovie($movie["id"]);
		$info[] = array(
			"id"=>$movie["id"],
			"title"=>$movie["title"],
			"poster_path"=>$movie["poster_path"],
			"release_date"=>$movie["release_date"],
			"genres"=>$details["genres"],
			"production_companies"=>$details["production_companies"]); 
	}
	//var_dump($response);
	return json_encode($info);
}

function displayWatchlist($movies = array()){
	$list = array();
	foreach($movies as $movie){
		$details = getMovie($movie);
		$list[] = array(
			"id" => $details["id"],
			"title" => $details["title"],
			"poster_path" => $details["poster_path"],
			"release_date" => $details["release_date"],
			"genres"=>$details["genres"],
			"production_companies"=>$details["production_companies"]);
	}
	print_r($list);
	return $list;
}

function getRecommended($id){
	$API_KEY = "f94eca9d744133b72f98995bdc4cca12";
	$url = "https://api.themoviedb.org/3/movie/$id/recommendations?api_key=$API_KEY&language=en-US";
	$options = array(
		CURLOPT_RETURNTRANSFER => true,
		//CURLOPT_HEADER => false,
		CURLOPT_FOLLOWLOCATION => true,
		CURLOPT_HTTPHEADER => array("Accept:application/json"));
	$recommend = curl_init($url);
	curl_setopt_array($recommend, $options);
	$result = curl_exec($recommend);
	curl_close($recommend);
	$result = json_decode($result,true);
	return $result;
}
?>
