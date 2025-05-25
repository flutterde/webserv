<?php
ob_start();

isset($_POST) ? print_r($_POST) : "Undefined";

function parse_cgi_input() {
    $input = '';
    if (isset($_SERVER['CONTENT_LENGTH']) && $_SERVER['CONTENT_LENGTH'] > 0) {
        $input = file_get_contents('php://input');
    }
    return $input;
}

function parse_cgi_cookies() {
    $cookies = [];
    if (isset($_SERVER['HTTP_COOKIE'])) {
        foreach (explode('; ', $_SERVER['HTTP_COOKIE']) as $cookie) {
            $parts = explode('=', $cookie, 2);
            if (count($parts) === 2) {
                $cookies[trim($parts[0])] = urldecode(trim($parts[1]));
            }
        }
    }
    return $cookies;
}

function handle_request() {
    $cookies = parse_cgi_cookies();
    
    if (!isset($cookies['session_id'])) {
        $cookies['session_id'] = bin2hex(random_bytes(16));
        header("Set-Cookie: session_id={$cookies['session_id']}; Path=/; HttpOnly; Max-Age=3600", false);
    }
    
    $post_data = [];
    if ($_SERVER['REQUEST_METHOD'] === 'POST') {
        $raw_input = parse_cgi_input();
        if (!empty($raw_input)) {
            parse_str($raw_input, $post_data);
        }
        
        if (isset($post_data['cookie_name']) && isset($post_data['cookie_value'])) {
            $cookies[$post_data['cookie_name']] = $post_data['cookie_value'];
            header("Set-Cookie: {$post_data['cookie_name']}={$post_data['cookie_value']}; Path=/", false);
        }
    }
    
    header('Content-Type: text/html');
    
    ?>
    <!DOCTYPE html>
    <html>
    <head>
        <title>CGI Request Information</title>
        <style>
            body { font-family: Arial, sans-serif; max-width: 800px; margin: 0 auto; padding: 20px; }
            pre { background: #f4f4f4; padding: 10px; overflow-x: auto; }
            table { border-collapse: collapse; width: 100%; }
            th, td { border: 1px solid #ddd; padding: 8px; text-align: left; }
        </style>
    </head>
    <body>
        <h1>CGI Request Information</h1>

        <h2>Cookies:</h2>
        <?php if (!empty($cookies)): ?>
            <table>
                <tr><th>Cookie Name</th><th>Value</th></tr>
                <?php foreach ($cookies as $name => $value): ?>
                    <tr><td><?= htmlspecialchars($name) ?></td><td><?= htmlspecialchars($value) ?></td></tr>
                <?php endforeach ?>
            </table>
        <?php else: ?>
            <p>No cookies found.</p>
        <?php endif ?>

        <h2>POST Data:</h2>
        <pre><?= htmlspecialchars(print_r($post_data, true)) ?></pre>

        <h2>Set a Cookie</h2>
        <form method="POST">
            <label>Cookie Name: <input type="text" name="cookie_name"></label><br>
            <label>Cookie Value: <input type="text" name="cookie_value"></label><br>
            <input type="submit" value="Set Cookie">
        </form>

        <h2>Server Environment:</h2>
        <pre><?php
        foreach ($_SERVER as $key => $value) {
            echo htmlspecialchars($key) . ": " . 
                (is_array($value) 
                    ? print_r($value, true) 
                    : htmlspecialchars($value)) . "\n";
        }
        ?></pre>
    </body>
    </html>
    <?php
}

handle_request();

ob_end_flush();
?>