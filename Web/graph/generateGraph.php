<?php
function generateGraphDynamic($datax,$datay, $filename){
	//Sletter den gamle filen hvis den eksisterer. 
	if(file_exists("graph/temp/$filename.jpeg")){
		unlink("graph/temp/$filename.jpeg");
	}
	//Lager et nytt Graph-objekt hvor konstruktøren har inn bildestørrelse som argumenter. 
	$graph = new Graph(1200,800);


	$timeObjs = array(); //Konverterer strenger til time-objekter.
	for($i = 0; $i < sizeof($datax);$i++){
		$tidn = strtotime($datax[$i]);
		array_push($timeObjs,$tidn);
	}
	//Setter skaleringen til aksene.
	$graph -> SetScale('datint',0,0,$timeObjs[0],end($timeObjs));
	$graph -> xaxis -> setFont(FF_FONT2,FS_BOLD,24);
	$graph -> yaxis -> setFont(FF_FONT2,FS_BOLD,24);
	$graph -> legend -> setFont(FF_FONT2,FS_BOLD,24);
	$graph -> yaxis -> title -> setFont(FF_FONT2, FS_BOLD,24);
	//$graph -> title -> Set("$filename");
	$graph -> xaxis->scale->SetTimeAlign(MINADJ_5);
	$graph -> xaxis->scale->ticks->Set(1*60*60);
	$graph -> xaxis->scale->SetDateFormat('H:i');
	$graph -> yaxis->scale->SetGrace(10,10);
	$graph -> yaxis->SetTitle("$filename",'middle');
	$graph -> yaxis->SetTitleMargin(50);
	$graph -> SetMargin(75,75,50,1);

	//Genererer tidspunkter på x-aksen
	$dateUtils = new DateScaleUtils();
	
	//Lager en ny linje og legger denne til i grafen. Kan potensielt har mange linjer i samme graf. 
	$lineplot = new LinePlot($datay, $timeObjs);
	$lastTimeStamp = end($datax);
	//$lineplot -> SetMargin(20,20,20,20);
	$lineplot -> SetLegend("$filename   fra   $datax[3]   til   $lastTimeStamp");
	$graph -> legend -> SetColor('#404040');
	$graph -> Add($lineplot); 
	$lineplot -> SetColor('#404040');
	//Skriver filen til temp-mappa. Like filnavn vil overskrives når funksjonen generateGraph() kalles.
	$graph->Stroke("graph/temp/$filename.jpeg");
	echo "<img src='graph/temp/$filename.jpeg' width='75%' height='75%'>";

}



function generateGraphStatic($datax,$datay, $filename){
	//Sletter den gamle filen hvis den eksisterer. 
	if(file_exists("graph/temp/$filename.jpeg")){
		unlink("graph/temp/$filename.jpeg");
	}
	
	//Lager et nytt Graph-objekt hvor konstruktøren har inn bildestørrelse som argumenter. 
	$graph = new Graph(800,640);


	$timeObjs = array(); //Konverterer strenger til time-objekter.
	for($i = 0; $i < sizeof($datax);$i++){
		$tidn = strtotime($datax[$i]);
		array_push($timeObjs,$tidn);
	}
	//Setter skaleringen til aksene.
	$graph -> SetScale('datint',-5,20,$timeObjs[0],end($timeObjs));
	$graph -> title -> Set("$filename");
	$graph -> xaxis->scale->SetTimeAlign(HOURADJ_1);
	$graph -> xaxis->scale->ticks->Set(60*60);
	$graph -> xaxis->scale->SetDateFormat('H:i');
	//$graph -> yaxis->scale->SetGrace(10,10);
	$graph ->SetClipping(true);


	//Genererer tidspunkter på x-aksen
	$dateUtils = new DateScaleUtils();
	$autoTicks = $dateUtils->getAutoTicks($timeObj[0],$timeObj[sizeof($timeObj)-1],4,false);
	$graph -> xaxis ->SetTickPositions($autoTicks[1]);
	
	//Lager en ny linje og legger denne til i grafen. Kan potensielt har mange linjer i samme graf. 
	$lineplot = new LinePlot($datay, $timeObjs);
	$lastTimeStamp = end($datax);
	$lineplot -> SetLegend("$filename   fra   $datax[3]   til   $lastTimeStamp");

	$graph -> Add($lineplot); 
	$lineplot -> SetColor('#CCAB00');
	//Skriver filen til temp-mappa. Like filnavn vil overskrives når funksjonen generateGraph() kalles.
	$graph->Stroke("graph/temp/$filename.jpeg");
	echo "<img src='graph/temp/$filename.jpeg'>";

}
?>


