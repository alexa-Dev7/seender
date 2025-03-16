<?php
session_start();

// Destroy session and redirect to login page
if (isset($_SESSION["username"])) {
    $username = $_SESSION["username"];

    // Load current sessions
    $sessions = json_decode(file_get_contents("sessions.json"), true);

    // Remove user’s session
    unset($sessions[$username]);
    file_put_contents("sessions.json", json_encode($sessions, JSON_PRETTY_PRINT));
}

// End session completely
session_unset();
session_destroy();

// Redirect to login
header("Location: login.php");
exit();
?>
