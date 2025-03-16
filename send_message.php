<?php
session_start();

if (!isset($_SESSION["username"])) {
    echo json_encode(["error" => "Unauthorized"]);
    exit();
}

$sender = $_SESSION["username"];
$recipient = $_POST["recipient"] ?? "";
$message = trim($_POST["message"] ?? "");

if (empty($recipient) || empty($message)) {
    echo json_encode(["error" => "Recipient and message are required."]);
    exit();
}

// Load encryption module
require_once("encrypt.php");

// Load messages JSON
$messages = json_decode(file_get_contents("messages.json"), true);

// Ensure message storage is structured
if (!isset($messages[$recipient])) {
    $messages[$recipient] = [];
}

// Encrypt message before storing
$encrypted_message = encrypt_message($message, $recipient);

// Push new message into the recipient's inbox
$messages[$recipient][] = [
    "from" => $sender,
    "message" => $encrypted_message,
    "timestamp" => date("Y-m-d H:i:s")
];

// Save updated message data
file_put_contents("messages.json", json_encode($messages, JSON_PRETTY_PRINT));

echo json_encode(["success" => "Message sent successfully"]);
exit();
?>
