<?php
require_once('rpc/path.inc');
require_once('get_host_info.inc');
require_once('rabbitMQLib.inc');

function sendDB($message) {
	$client = new rabbitMQClient('testRabbitMQ.ini','testServer');
	$response = $client->send_request($message);
	return $response;
}
function sendAPI($message) {
	$client = new rabbitMQClient('testRabbitMQ.ini','dmz');
	$response = $client->send_request($message);
	return $response;
}
function sendLog($message) {
	$client = new rabbitMQClient('testRabbitMQ.ini', 'Logger');
	$date = date_create('now', timezone_open('America/New_York'));
	$timeMessage = date_format($date, 'H:i:sa') . ':';
	$client->publish($timeMessage . $message, 3000);
}
?>
