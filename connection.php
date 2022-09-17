<?php
$username = "root";
$pass = "";
$host = "localhost";
$db_name = "antifurto";
$con = mysqli_connect ($host, $username, $pass);
$db = mysqli_select_db ( $con, $db_name );
?>