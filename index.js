require("dotenv").config(); // Load .env file

const PORT = process.env.NODE_PORT || 8081;
const RSA_KEY_SIZE = parseInt(process.env.RSA_KEY_SIZE) || 2048;
const MESSAGE_DB = process.env.MESSAGES_DB || "messages.json";

// Import Node modules
const express = require("express");
const bodyParser = require("body-parser");
const fs = require("fs");
const crypto = require("crypto");

// Setup the Express app
const app = express();
app.use(bodyParser.json());

// RSA key generation for E2EE
const { publicKey, privateKey } = crypto.generateKeyPairSync("rsa", {
  modulusLength: RSA_KEY_SIZE,
});

// Endpoint to get the public key
app.get("/getPublicKey", (req, res) => {
  res.json({ publicKey: publicKey.export({ type: "pkcs1", format: "pem" }) });
});

// Endpoint to handle receiving encrypted messages
app.post("/sendEncryptedMessage", (req, res) => {
  const { user, encryptedMessage } = req.body;

  if (!user || !encryptedMessage) {
    return res.status(400).json({ error: "Invalid data" });
  }

  let messages = [];

  // Load existing messages if available
  if (fs.existsSync(MESSAGE_DB)) {
    messages = JSON.parse(fs.readFileSync(MESSAGE_DB));
  }

  messages.push({
    user,
    encryptedMessage,
    timestamp: new Date().toISOString(),
  });

  // Save messages back to the file
  fs.writeFileSync(MESSAGE_DB, JSON.stringify(messages, null, 2));

  res.json({ status: "Message received securely" });
});

// Start Node backend server
app.listen(PORT, () => {
  console.log(`Node.js backend running on port ${PORT}`);
});
