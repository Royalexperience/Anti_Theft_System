<?php
$url=$_SERVER['REQUEST_URI'];
header("Refresh: 5; URL=$url"); // Aggiorna la pagina ogni 5 secondi
?>
<html>
<head>
<title>Esp32 Anti-Furto Distanze Semi-Critiche</title>
<style type="text/css">
.table_titles {
padding-right: 20px;
padding-left: 20px;
color: #000;
}
.table_titles {
color: #FFF;
background-color: #0000FF;
}
table {
border: 2px solid #333;
}
body { font-family: "Trebuchet MS", Courier; }
</style>
</head>
<body>
<h1>Esp32 Anti-Furto Distanze Semi-Critiche</h1>
<table border="0" cellspacing="0" cellpadding="4">
<tr>
<td class="table_titles">ID</td>
<td class="table_titles">DISTANZA</td>
<td class="table_titles">ORARIO</td>
</tr>
<?php
include('connection.php');
$result = mysqli_query($con,'SELECT * FROM distanze ORDER BY id DESC');

// Elaborazione di ciascun record
$oddrow = true;
while($row = mysqli_fetch_array($result))
{
if ($oddrow)
{
$css_class=' class="table_cells_odd"';
}
else
{
$css_class=' class="table_cells_even"';
}
$oddrow = !$oddrow; 
echo "<tr>";
echo "<td '.$css_class.'>" . $row['id'] . "</td>";
echo "<td '.$css_class.'>" . $row['distanza'] . "</td>";
echo "<td '.$css_class.'>" . $row['orario'] . "</td>";
echo "</tr>"; 
}
 
// Chiudo La connessione
mysqli_close($con);
?>
</table>
</body>
</html>