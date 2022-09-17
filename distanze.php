<?php
include ('connection.php');
$sql_insert = "INSERT INTO distanze (distanza) VALUES ('".$_GET["distanza"]."')";
if(mysqli_query($con,$sql_insert))
{
echo "fatto";
mysqli_close($con);
}
else
{
echo "error is ".mysqli_error($con );
}
?>