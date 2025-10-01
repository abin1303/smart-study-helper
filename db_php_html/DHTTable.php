<!DOCTYPE html>
<html>
<head>
	<meta charset = "UTF-8">
	<meta http-equiv = "refresh" content = "30">
	<style type = "text/css">
		.spec{
			text-align:center;
		}
		.con{
			text-align:left;
		}
		</style>
</head>

<body>
	<h1 align = "center">TEMP HUMI</h1>
	<div class = "spec">
		# <b>Condition of library</b>
		<br></br>
	</div>

	<table border = '1' style = "width = 30%" align = "center">
	<tr align = "center">
		<th>date</th>
		<th>time</th>
		<th>humi</th>
		<th>temp</th>

	<?php
		$conn = mysqli_connect("localhost", "iot", "pwiot");
		mysqli_select_db($conn, "Library_Member");
		$result = mysqli_query($conn, "select * from DHT_data");
		while($row = mysqli_fetch_array($result))
		{
			echo "<tr align = center>";
			echo '<th>'.$row['date'].'</td>';
			echo '<th>'.$row['time'].'</td>';
			echo '<th>'.$row['humi'].'</td>';
			echo '<th>'.$row['temp'].'</td>';

			echo "</tr>";

		}
		mysqli_close($conn);
	?>
	</table>
</body>
</html>
