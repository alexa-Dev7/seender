<?php
session_start();

// Handle login form submission
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $username = $_POST["username"] ?? "";
    $password = $_POST["password"] ?? "";

    $users = json_decode(file_get_contents("users.json"), true);

    // Check if user exists and password matches
    if (isset($users[$username]) && password_verify($password, $users[$username]["password"])) {
        $_SESSION["username"] = $username;

        // Track session
        $sessions = json_decode(file_get_contents("sessions.json"), true);
        $session_id = session_id();
        $sessions[$username] = $session_id;

        file_put_contents("sessions.json", json_encode($sessions, JSON_PRETTY_PRINT));

        header("Location: index.php");
        exit();
    } else {
        $error = "Invalid username or password.";
    }
}

// Handle registration
if (isset($_POST["register"])) {
    $username = $_POST["username"] ?? "";
    $password = $_POST["password"] ?? "";

    $users = json_decode(file_get_contents("users.json"), true);

    if (!isset($users[$username])) {
        $hashed_password = password_hash($password, PASSWORD_BCRYPT);
        $users[$username] = ["password" => $hashed_password];

        file_put_contents("users.json", json_encode($users, JSON_PRETTY_PRINT));
        $success = "Account created successfully. Please login.";
    } else {
        $error = "Username already exists.";
    }
}
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="assets/reset.css">
    <link rel="stylesheet" href="assets/styles.css">
    <title>Login | Messenger Clone</title>
</head>
<body>

<div class="login-container">
    <h1>Login</h1>
    <?php if (isset($error)): ?>
        <p class="error"><?= htmlspecialchars($error) ?></p>
    <?php endif; ?>
    <?php if (isset($success)): ?>
        <p class="success"><?= htmlspecialchars($success) ?></p>
    <?php endif; ?>

    <form action="login.php" method="post">
        <input type="text" name="username" placeholder="Username" required>
        <input type="password" name="password" placeholder="Password" required>
        <button type="submit">Login</button>
    </form>

    <h2>Or Register</h2>
    <form action="login.php" method="post">
        <input type="hidden" name="register" value="1">
        <input type="text" name="username" placeholder="Username" required>
        <input type="password" name="password" placeholder="Password" required>
        <button type="submit">Register</button>
    </form>
</div>

</body>
</html>
