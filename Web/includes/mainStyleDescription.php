<?php
/*
"body" gjelder for alt som står mellom <body> og </body>
Setter font, skriftstørrelse, men de andre verdiene den setter blir som oftest overskrevet av andre classer

".furthest-back" er klasse for bakgrunnen og i mainExample.php er body tilordnet denne klassen. <body class="furthest-back"></body>

".header-cont" er klasse for beholderen til headeren. 
"position:fixed" gjør at denne er som limt til toppen av skjermbildet selv når du scroller.

".header" er klasse for selve boksen som headeren ligger i.

".content" er klasse for hovedblokkene av innhold. Alt som ikke er tilordnet ".header" eller ".footer" burde ligge i ".content" slik at det ikke blir liggende direkte på bakgrunnen.

".content-header" er klasse for overskrifter (headere) i ".content"-blokker.

".content-text" er klasse for tekst og annet i ".content".

".graph" burde tilordnes til grafer slik at de blir midtstilt.

".nav" (med underklasser li og a) er klasse for lister som blir brukt i header og footer.

".logout" er en lignende klasse, men denne har float:right (høyrestilt) slik at logg ut blir penere plassert i header.

".footer" er klasse for bunnteksten.

".led" (og alle fargevariasjonene) er klasser for statuslyset.

"#temp", "#vann" osv er identiteter (id).
Ved å legge til f.eks <div id="temp"></div> over der hvor vi har statistikk for temperatur, og sette navigasjonsknappen "Temperatur" til å linke til denne id-en,
kan vi opprette enkel navigasjon på siden.

".error" er en klasse for feilmelding.
*/
?>
