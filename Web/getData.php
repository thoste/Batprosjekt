<?php
include "includes/connect_w.php";
function getData_4($input){

	if(!function_exists('connect_4')){
		echo "Error: connect_4 does not exist!";
		return;
	}

	$conn = connect_4();

	if ($conn->connect_error){
		die("Connection failed: " . $conn->connect_error);
        }
	$sql_query = "INSERT INTO baat(";
	$values = "VALUES (";

	$row = "";
	$terminatorPos = 0;
	$terminator = false;
	$numlines = 0;

	$timeStamp = 0;
	$timeStampBytes = array();
	$timeStampString = "";

      while(1){
                //We make a substring of the first 4 entries that contains the timestamp.
                if(strlen($input) >= 7){
                        $timeStampString = substr($input, 0, 4);
                        $input = substr($input, 4);
                }else{
                        break;
                }

                for($w=0; $w<4; $w++){
                        $timeStampBytes[$w] = ord($timeStampString[$w]);
                }

                $timeStamp += $timeStampBytes[0]<<24;
                $timeStamp += $timeStampBytes[1]<<16;
                $timeStamp += $timeStampBytes[2]<<8;
                $timeStamp += $timeStampBytes[3];
		
		//The timeStamp from the Arduino is in Unix-time, with timezone adjustments, but not daylight saving adjustments.
		//time("I") returns true if the current day is in daylight savings. If so, we add an hour to the Unix-time from Arduino.
		if(time("I")){
			$timeStamp+=3600;
		}

                $sql_query.="timestamp, ";
                $values.="'".$timeStamp."',";

                //We make a substring of the sensor data. First we need to find the position for the terminator value.
                for($i = 0; $i < strlen($input); $i++){
                        if(ord($input[$i]) == 99 && ($i % 2 == 0)){
                                $terminatorPos = $i;
                                $terminator = true;
                                break;
                        }
                }

                if($terminator){
                        $row = substr($input, 0, $terminatorPos);
                        $input = substr($input, $terminatorPos+1);//$terminatorPos+1 so that we dont add the terminating value.
                        $numlines++;
                        //We have a full row and wish to insert it in the database.

                        for($v = 1; $v < strlen($row); $v+=2){
                                $s = $v-1;
                                $sensorType = ord($row[$s]);
                                $sensorValue = ord($row[$v]);

                                //We switch through the sensors to add the matching string for the sensor type
                                switch($sensorType){
                                case 2:
                                        $sql_query.="temperatur, ";
                                        $values.="'".$sensorValue."',";
                                        break;
                                case 3:
                                        $sql_query.="vannhoyde, ";
                                        $values.="'".$sensorValue."',";
                                        break;
                                case 4:
                                        $sql_query.="pumpe, ";
                                        $values.="'".$sensorValue."',";
                                        break;
                                case 5:
                                        $sql_query.="krengningX, ";
                                        $values.="'".$sensorValue."',";
                                        break;
                                case 6:
                                        $sql_query.="krengningY, ";
                                        $values.="'".$sensorValue."',";
                                        break;
                                case 7:
                                        $sql_query.="hoyde, ";
                                        $values.="'".$sensorValue."',";
                                        break;
                                case 8:
                                        $sql_query.="vind, ";
                                        $values.="'".$sensorValue."',";
                                        break;
                                case 9:
                                        $sql_query.="status, ";
                                        $values.="'".$sensorValue."',";
                                        break;
                                case 10:
                                        $sql_query.="snolast, ";
                                        $values.="'".$sensorValue."',";
                                        break;
                                case 11:
                                        $sql_query.="landstrom, ";
                                        $values.="'".$sensorValue."',";
                                        break;
                                case 12:
                                        $sql_query.="tyveri, ";
                                        $values.="'".$sensorValue."',";
                                        break;
                                case 13:
                                        $sql_query.="brann, ";
                                        $values.="'".$sensorValue."',";
                                        break;
                                default:
                                        break;
                                }//end switch
                        }//end for loops through the row

                        //After the for loop we combine $sql_query and $values and get
                        //a proper mysql command for adding the row
                        $sql_query = trim($sql_query, ", ").")";
                        $sql_query .= " ".trim($values,",").");";

                        //If the command is not empty
                        if($sql_query != 'INSERT INTO baat() VALUES ();'){
                                //We submit it and get a boolean true if everything went ok.
                                if(mysqli_query($conn, $sql_query)){
                                        //echo $sql_query." ";
                                        //echo "added sucessfully! \n";
                                }else{
                                        //echo "Error: " . $sql_query . "<br>" . mysqli_error($conn);
                                }
                        }
                        //Then we reset the strings so that they are ready to be filled again
                        $sql_query = "INSERT INTO baat(";
                        $values = "VALUES (";
                        $terminator = false;
                        $timeStamp = 0;

                }//end if(terminator). We are now finished adding the row
                else{//If we did not find a terminator
                        break;
                }

        }//end while
	echo "\nClient added $numlines rows to the database.";
}//End function
?>
