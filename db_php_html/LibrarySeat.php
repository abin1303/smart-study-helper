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
	<h1 align = "center">Available Seats</h1>
	<div class = "spec">
		# <b>you can choose seat available = 1</b>
		<br></br>
	</div>

	<table border = '1' style = "width = 30%" align = "center">
	<tr align = "center">
		<th>seat_no</th>
		<th>available</th>
	</tr>

	<?php
		$conn = mysqli_connect("localhost", "iot", "pwiot");
		mysqli_select_db($conn, "Library_Member");
		$result = mysqli_query($conn, "select * from seat_use");
		while($row = mysqli_fetch_array($result))
		{
			echo "<tr align = center>";
			echo '<th>'.$row['seat_no'].'</td>';
			echo '<th>'.$row['available'].'</td>';
			echo "</tr>";

		}
		mysqli_close($conn);
	?>
	</table>
</body>
</html>
