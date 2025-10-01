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
	<h1 align = "center">Library Using Data</h1>
	<div class = "spec">
		# <b>using reading room</b>
		<br></br>
	</div>

	<table border = '1' style = "width = 30%" align = "center">
	<tr align = "center">
		<th>NUM</th>
		<th>DATE</th>
		<th>StartTime</th>
		<th>EndTime</th>
		<th>UsingTime</th>
		<th>StudyTime</th>
		<th>UsedSeat</th>
	</tr>

	<?php
		$conn = mysqli_connect("localhost", "iot", "pwiot");
		mysqli_select_db($conn, "Library_Member");
		$result = mysqli_query($conn, "select * from MEMBER");
		while($row = mysqli_fetch_array($result))
		{
			echo "<tr align = center>";
			echo '<th>'.$row['NUM'].'</td>';
			echo '<th>'.$row['DATE'].'</td>';
			echo '<th>'.$row['StartTime'].'</td>';
			echo '<th>'.$row['EndTime'].'</td>';
			echo '<th>',$row['UsingTime'].'</td>';
			echo '<th>',$row['StudyTime'].'</td>';
			echo '<th>',$row['UsedSeat'].'</td>';

			echo "</tr>";

		}
		mysqli_close($conn);
	?>
	</table>
</body>
</html>
