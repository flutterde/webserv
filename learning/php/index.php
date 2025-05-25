<?php
// Set a cookie that expires in 1 hour
if (!isset($_COOKIE['user'])) {
    setcookie('user', 'JohnDoe', time() + 3600, '/'); // Cookie lasts for 1 hour
    setcookie('name', 'Anwar', time() + 3600, '/'); // Cookie lasts for 1 hour
    echo "Cookie 'user' has been set!";
} else {
    echo "Welcome back, " . $_COOKIE['user'] . "!";
}
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Simple PHP Cookie Example</title>
</head>
<body>
    <h1>PHP Cookie Example</h1>
    <p>The PHP script above checks for a cookie and sets it if not already set.</p>
</body>
</html>
