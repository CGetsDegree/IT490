#!/usr/bin/php
<?php

require_once('path.inc');
require_once('get_host_info.inc');
require_once('rabbitMQLib.inc');

function process($error) {
	$handler = fopen("errorLog.txt","a+");
	fwrite($handler, $error."\n");
	fclose($handler);
	echo 'Log recieved'.PHP_EOL;
}
$server = new rabbitMQServer("testRabbitMQ.ini","Logger");
echo "Logging started up".PHP_EOL;
$server->process_requests('process');
?>
