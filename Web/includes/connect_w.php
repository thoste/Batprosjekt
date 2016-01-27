<?php

//Funksjonen "connect" returnerer en tilkobling til mysql databasen på serveren

function connect_4(){

	$servername = "localhost";
	$username = "writedb4";
	$password = "aSKP45rQUp7A";
	$dbname = "gruppe4";

	$conn = new mysqli($servername, $username, $password, $dbname);
	if(!$conn){
		echo"Did not connect <br>";
	}else{

	return $conn;
	
	}
}

?>
