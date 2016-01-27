<?php

//Funksjonen "connect" returnerer en tilkobling til mysql databasen på serveren

function connect(){

	$servername = "localhost";
	$username = "readdb";
	$password = "CcCturrzSZBw";
	$dbname = "gruppe4";

	$conn = new mysqli($servername, $username, $password, $dbname);
	if(!$conn){
		echo"Did not connect <br>";
	}else{

	return $conn;
	
	}
}

?>
