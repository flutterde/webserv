<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>PHP | 1337 | Webserv</title>
</head>
<body>
	<center>
		<?php
			$x = 1337;
			$y = 42;
			echo "<h2>$x is impair</h2>";
			echo "<h2>$y is pair</h2>";
			$random_number = rand(1, 100);
			if ($random_number % 2 == 0) {
				echo "<h2 style=\"color: red\">$y win beceause the number is: $random_number, So it's pair</h2>";
			} else {
				echo "<h2  style=\"color: green\">$x win beceause the number is: $random_number, So it's impair</h2>";
			}
		?>
	</center>
</body>
</html>