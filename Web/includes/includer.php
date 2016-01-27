<?php
/*
Denne filen inkluderer alle filene vi ønsker å inkludere.
Ved å kun inkludere includer.php som ligger i includes mappen har man da inkludert alle filene som inkluderes her.
F.eks include("includes/includer.php");
Vi hadde problemer med å sette et default include directory for php og dermed gjorde vi det heller på denne måten.
*/
include("includes/cssStyle.php");
include("includes/functions.php");
include("includes/connect_r.php");
include("includes/timeconvert.php");
//include("includes/graphConnect.php");
include("graph/jpgraph/src/jpgraph.php");
include("graph/jpgraph/src/jpgraph_line.php");
include("graph/jpgraph/src/jpgraph_date.php");
include("graph/jpgraph/src/jpgraph_utils.inc.php");
include("graph/generateGraph.php");
?>

