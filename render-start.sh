#!/bin/bash

# Set environment variables for Node and C++ servers
export NODE_PORT=8081
export CPP_PORT=8080

# Ensure the script exits on any error
set -e

# Log output for visibility
echo "Starting SenderChatApp..."

# Launch C++ backend
echo "Launching C++ backend on port $CPP_PORT..."
./server &

# Launch Node.js backend
echo "Launching Node.js backend on port $NODE_PORT..."
node index.js &

# Keep everything running
wait
