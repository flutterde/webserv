<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title><?php echo "PHP CGI | 1337" ?></title>
</head>
<body>
	<center><h1><?php $tm = date("H"); 
	if ($tm < "12") { echo "Good Morning Albatal"; } 
	elseif ($tm < "17") { echo "Good Afternoon Albatal"; } 
	else { echo "Good Evening Albatal";}?></h1></center>
	<p>
		<hr>
		<?php
		$name = $_GET['NAME'];
		echo "<br><br>Hi, $name";
		?>
	</p>
</body>
</html>