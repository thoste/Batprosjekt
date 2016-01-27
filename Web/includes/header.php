<?php
/*
Denne filen oppretter en header som inneholder navigasjonslinker og logg ut knapp.
I mainExample.php er dette det første som skjer i <body>, ved "include("includes/header.php")".
Da dette er et rent script (ikke en funksjon) vil det kjøre med en gang der hvor det inkluderes.
Headeren vil følge toppen av skjermen, selv når du scroller på siden.
*/
?>

<div class="header-cont">
	<div class="header">
		<ul class="nav">
		<li><a href="#status">Status</a></li>
		<li><a href="#temp">Temperatur</a></li>
		<li><a href="#vind">Vind</a></li>
		<li><a href="#vann">Vannhøyde</a></li>
		<li><a href="#krengning">Krengning</a></li>
		<li><a href="#snolast">Snølast</a></li>
		</ul>
		<ul class="logout">
		<li><a href="../login_includes/logout.php">Logg ut</a></li>
		</ul>
	</div>
</div>


