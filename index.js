require("dotenv").config(); // Load .env

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

// Port setup (matches Dockerfile)
const PORT = 8081;

// RSA key generation for E2EE
const { publicKey, privateKey } = crypto.generateKeyPairSync("rsa", {
  modulusLength: 2048,
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

  const messagesFile = "messages.json";
  let messages = [];

  // Load existing messages if available
  if (fs.existsSync(messagesFile)) {
    messages = JSON.parse(fs.readFileSync(messagesFile));
  }

  messages.push({ user, encryptedMessage, timestamp: new Date().toISOString() });

  // Save messages back to the file
  fs.writeFileSync(messagesFile, JSON.stringify(messages, null, 2));

  res.json({ status: "Message received securely" });
});

// Start Node backend server
app.listen(PORT, () => {
  console.log(`Node.js backend running on port ${PORT}`);
});
