<?php
/**
 * Created by PhpStorm.
 * User: Jomar
 * Date: 21.01.2016
 * Time: 21.32
 */
//Spesifiserer egenskaper for headeren.

include("../login_includes/functions.php");
include("../login_includes/db_connect.php");
//include("login_includes/login_includer.php");
sec_session_start();

?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" href="../../favicon.ico">
    <title>Starter Template for Bootstrap</title>
    <link href="node_modules/bootstrap/dist/css/bootstrap.min.css" rel="stylesheet">
    <link href="starter-template.css" rel="stylesheet">
    <script src="https://oss.maxcdn.com/html5shiv/3.7.2/html5shiv.min.js"></script>
    <script src="https://oss.maxcdn.com/respond/1.4.2/respond.min.js"></script>
    <link rel="stylesheet" type="text/css" href="style.css">

</head>

<body>
<?php if(login_check($mysqli) == "gruppe4") :/* VIKTIG! Alt som kommer etter denne kodelinja må man være innlogget for å  se*/ ?>

<nav class="navbar navbar-inverse navbar-fixed-top">
    <div class="container">
        <div class="navbar-header">
            <button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#navbar" aria-expanded="false" aria-controls="navbar">
                <span class="sr-only">Toggle navigation</span>
                <span class="icon-bar"></span>
                <span class="icon-bar"></span>
                <span class="icon-bar"></span>
            </button>
            <a class="navbar-brand" href="#">Project name</a>
        </div>
        <div id="navbar" class="collapse navbar-collapse">
            <ul class="nav navbar-nav">
                <li class="active"><a href="#">Home</a></li>
                <li><a href="#about">About</a></li>
                <li><a href="#contact">Contact</a></li>
            </ul>
        </div>
    </div>



</nav>

    <div id="center">
    <div class = "status-box">
        <h1>Temperatur test test</h1>
        <p>Motorrom: 5 &deg;C</p>
        <p>Kabin: 17 &deg;C</p>
    </div>
    <div class = "status-box">
        <h1>Brann</h1>
        <div class = status-light>

        </div>

    </div>
    <div class = "status-box">
        <h1>Vannstand</h1>
    </div>
    <div class = "status-box">
 