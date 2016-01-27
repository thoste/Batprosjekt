<?php
//Spesifiserer egenskaper for headeren.
header("Cache-Control: no-cache");
header("Cache-Control: no-store");
header("Cache-Control: must-revalidate");
header("Pragma: no-cache");
header("Expires: 0");
include("../login_includes/functions.php");
include("../login_includes/db_connect.php");
//include("login_includes/login_includer.php");
sec_session_start();
?>

<!DOCTYPE html>

<html>

<head>
	<title>Eksempelbåt</title>
	
	<?php
	//Inkluderer "includer.php" som ligger i mappen "includes",
	//for å benytte funksjonalitet som ligger i filene som "includer.php" lister opp. 
	include("includes/includer.php");
	
	//Funksjonen "connect()" ligger i "connect.php".
	//Den returnerer en tilkobling til databasen.
	$conn = connect();
	?>

</head>
<?php if(login_check($mysqli) == "gruppe4") :/* VIKTIG! Alt som kommer etter denne kodelinja må man være innlogget for å  se*/ ?> 
<?php /*
<body> er tilordnet klassen "furthest-back".
Denne klassen setter bakgrunnsfargen, som er en fade fra en lysere blåfarge til en mørkere blåfarge.
*/ ?>

	<body class="furthest-back">

	<?php /*
	Inkluderer "header.php" som oppretter navigasjonslinjen øverst på nettsiden.
	For mer informasjon se i "header.php".
	Da header.php ikke inneholder funksjoner, men er et rent script,
	vil dette være det samme som om vi hadde kopiert det som stod i filen, og limt det inn her.
	*/
	include("includes/header.php"); 
	?>

	<?php /*
	Oppretter en blokk av typen <div> tilordnet klassen "content".
	Klassen "content" former blokken slik at den er midtstilt, dekker ca halvparten av skjermen
	og har en hvit farge. 
	*/ ?>
	<div class="content">

	<?php /*
	Klassen "content-header" former blokken med "Velkommen!" slik at den har en større skriftstørrelse,
	mørkere bakgrunnsfarge og en mørkere kant oppe og nede.
	Denne klassen bør benyttes på alle overskrifter i "content".
	*/ ?>
	<div class="content-header">Velkommen!</div>
		<?php
		//Klassen "content-text" former teksten i denne blokken slik at den har passe marginer.
		?>
		<div class="content-text">
		Her finner du en fullstendig oversikt over statistikken på din båt.<br>
		Bruk navigasjonslinjen over for å navigere på siden.
		</div>

	<?php /*
	Legger inn en id "status" slik at vi kan navigere hit.
	I "header.php" kan du se at knappen "Status" er en link til id-en "status" (href = "#status"),
	og ved å trykke på knappen "Status" vil vi bli sendt til denne blokken på siden.
	*/ ?>
	<div id="status"><br><br></div>
	<div class="content-header">

		<?php
		//Kaller på funksjonen "getLastStatus" for å hente ut siste status-variabel i tabellen 'eksempelbaat',	
		$status = getLastStatus($conn, 'baat'); 

		//Bruker $status-variabelen til å gi riktig farge på statuslyset.
		//Blokken som er statuslyset er tilordenet to stilklasser, "led" og "led-color".
		?>
		Status <div class="led led-<?php echo"$status" ?>"></div>
	</div>

	<br>
	<?php 
	//Oppretter en tabell med siste måling.
	createLastMeasure($conn, 'baat'); 
	?>

	</div>	

	<div class="content">

	<div class="content-header">Valg av tidsintervall</div>

	<?php
	//Includerer scriptet form.php som oppretter et form hvor brukeren kan velge tidsintervall.
	//I dette scriptet opprettes også arrayen $timeRange som holder på tidsintervallet.
	include("includes/form.php");
	?>

	<div id="temp"></div>

	</div>

	<div class="content">

	<div class="content-header">Temperatur</div>
		
		<?php
		//Stilklassen "graph" er beregnet for blokker med grafer, og sentrerer bildene.
		?>
		<div class="graph">

		<?php 
		//Printer til nettsiden.
		//Se kommentarer i implementasjonen i functions.php for mer informasjon.
		printGraph($conn, 'baat', 'Temperatur', $timeRange, 'temperatur', 'dynamic'); 
		?>
		</div>
		
	<?php
	//id for navigasjon, overskrift "Vind" og graf.
	?>
	<div id="vind"><br><br></div>
	<div class="content-header">Vind</div>
		<div class="graph">
		<?php printGraph($conn,'baat','Vindstyrke',$timeRange,'vind', 'dynamic'); ?>
		</div>
	<?php
	//id for navigasjon, overskrift "Vannhøyde" og graf.
	?>
	<div id="vann"><br><br></div>
	<div class="content-header">Vannhøyde</div>
		<div class="graph">
		<?php createPumpTable($conn,'baat',$timeRange,'vannhoyde'); ?>
		</div>


	<div id="krengning"><br><br></div>
	<div class="content-header">Krengning</div>
	<br><br><br><br><br><br><br>

	<div id="snolast"><br><br></div>
	<div class="content-header">Snølast</div>
	<br><br><br><br><br><br><br>

	<?php 
	//Inkluderer footer
	include("includes/footer.php"); 

	//Lukker tilkoblingen til databasen.
	$conn->close();
	?>
	</div>
	
	<?php else : /*Dette vises hvis en bruker forsøker å få tilgang til siden uten å være innlogget*/?>
		Du er ikke innlogget.
	<?php endif; ?>
</body>
</html>

