<?php
// Check if the form is submitted
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Retrieve the cookie name and value from the form
    $cookie_name = $_POST['cookie_name'] ?? '';
    $cookie_value = $_POST['cookie_value'] ?? '';

    echo $cookie_name;
    // Set the cookie if both name and value are provided
    if (!empty($cookie_name) && !empty($cookie_value)) {
        setcookie($cookie_name, $cookie_value, time() + 3600, "/"); // Cookie expires in 1 hour
        echo "Cookie '{$cookie_name}' set successfully!<br>";
    } else {
        echo "Please provide both cookie name and value.<br>";
    }
}
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Set Cookie</title>
</head>
<body>
    <pre>
        <?php print_r($_SERVER); ?>
    </pre>
    <form method="POST">
        <label>Cookie Name: <input type="text" name="cookie_name"></label><br>
        <label>Cookie Value: <input type="text" name="cookie_value"></label><br>
        <input type="submit" value="Set Cookie">
    </form>
</body>
</html>