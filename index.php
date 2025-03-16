<?php
session_start();

// Check for active session
if (!isset($_SESSION['username'])) {
    header("Location: login.php");
    exit();
}

$username = $_SESSION['username'];

// Fetch active users (simulate short polling)
function getActiveUsers() {
    $users = json_decode(file_get_contents("users.json"), true);
    $sessions = json_decode(file_get_contents("sessions.json"), true);

    $active_users = array_keys($sessions);
    return array_filter($active_users, fn($user) => $user !== $_SESSION['username']);
}

$active_users = getActiveUsers();
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="assets/reset.css">
    <link rel="stylesheet" href="assets/styles.css">
    <title>Messenger Clone</title>
</head>
<body>

<div class="container">
    <h1>Welcome, <?= htmlspecialchars($username) ?></h1>
    <div class="users-list">
        <h2>Active Users</h2>
        <?php foreach ($active_users as $user): ?>
            <div class="user">
                <span><?= htmlspecialchars($user) ?></span>
                <form action="send_message.php" method="post">
                    <input type="hidden" name="recipient" value="<?= htmlspecialchars($user) ?>">
                    <button type="submit">Message</button>
                </form>
            </div>
        <?php endforeach; ?>
    </div>

    <form action="logout.php" method="post">
        <button type="submit" class="logout-btn">Logout</button>
    </form>
</div>

</body>
</html>
