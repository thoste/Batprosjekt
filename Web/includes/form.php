<?php
//Dersom man velger tidsintervall, vil arrayen $timeRange holde på det nye tidsintervallet.
$timeRange = getTimeRange();

//Oppretter en form hvor brukeren kan skrive inn ønsket tidsintervall.
?>
<form action="<?php echo $_SERVER['PHP_SELF']; ?>" method="post">
	<?php 
	$to = date('Y-m-d H:i:s');
	$from = get_X_days_ago(2);
	
	str_replace('index_old.php', '', $_SERVER['PHP_SELF']);
	?>
	
	<div class="content-text">	
	<label for = "from">Fra:</label>
	<input type = "text" name = "from" value = <?php echo "'$from'"; ?> >
	<label for = "to">Til:</label>
	<input type = "text" name = "to" value = <?php echo "'$to'"; ?> >
	<input type = "submit" id = "button" name = "SubmitButton">
	</div>
</form>


