<?php

/*
Denne filen inneholder mange funksjoner som tilsammen gjør det mulig å skrive om fra ntp tid til datetime med funksjonen get_date($antall_sek_etter_1900).
Altså fra formen "antall sekunder etter 1.januar 1900" til formen "yyyy-mm-dd hh:mm:ss".
*/

function isLeapYear($year){
if(($year % 400) == 0){
	return true;
}else if(($year % 100) == 0){
	return false;
}else if(($year % 4) == 0){
	return true;
}else{
	return false;
}
}

function daysInMonth($month, $year){
if(($month < 1) || ($month > 12)){
	return 0;
}
switch($month){
case 2:
	if(isLeapYear($year)){
		return 29;
	}else{
		return 28;
	}
case 1:
case 3:
case 5:
case 7:
case 8:
case 10:
case 12:
	return 31;
default:
	return 30;
}
}

function secsInMonth($month, $year){
	$DAYSECONDS = 86400;
	return (daysInMonth($month, $year) * $DAYSECONDS);
}

function secsInYear($year){
	$DAYSECONDS = 86400;
	if(isLeapYear($year)){
		return 366 * $DAYSECONDS;
	}else{
		return 365 * $DAYSECONDS;
	}
}

//GET_DATE

function get_date($secsSince1900){
$DAYSECONDS = 86400;
//2208988800 seconds between 1970 and 1900
//2 hours = 3600*2
$mux = 1;
if(date("I")){
	$mux ++;
}

$secs = $secsSince1900 - 2208988800 + (3600*$mux);
$year = 1970;

//Finds year
while($secs >= secsInYear($year)){
	$secs -= secsInyear($year);
	$year++;
}

//Finds month
$month = 1;
while($secs >= secsInMonth($month, $year)){
	$secs -= secsInMonth($month, $year);
	$month++;	
}
if($month < 10){ $month = "0$month"; }

//Finds day
$float_day = ($secs / $DAYSECONDS) + 1;
$day = floor($float_day);
if($day < 10){ $day = "0$day"; } 

//Finds hour
$frac_day = $float_day - $day;
$float_hour = $frac_day * 24;
$hour = floor($float_hour);
if($hour < 10){ $hour = "0$hour"; }

//Finds minute
$frac_hour = $float_hour - $hour;
$float_minute = $frac_hour * 60;
$minute = floor($float_minute);
if($minute < 10){ $minute = "0$minute"; }

//Finds second
$frac_minute = $float_minute - $minute;
$float_second = $frac_minute * 60;
$second = floor($float_second);
if($second < 10){ $second = "0$second"; }

//return it all
return "$year-$month-$day $hour:$minute:$second";

}




