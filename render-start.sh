#!/bin/bash

# Exit on any error
set -e

# Show startup logs for visibility
echo "🚀 Starting Sender Chat App..."

# Ensure environment variables are set
export NODE_PORT=8081
export CPP_PORT=8080

# Launch the C++ server in the background
echo "⚙️ Launching C++ backend server on port $CPP_PORT..."
./server &
CPP_PID=$!

# Launch Node.js backend in the background
echo "⚙️ Launching Node.js backend on port $NODE_PORT..."
node index.js &
NODE_PID=$!

# Function to handle termination (CTRL+C or container stop)
cleanup() {
    echo "🛑 Shutting down servers..."
    kill $CPP_PID $NODE_PID
    exit 0
}

# Trap exit signals and run cleanup
trap cleanup SIGINT SIGTERM

# Keep the script running (prevents container exit)
echo "✅ All servers running! Press Ctrl+C to stop."
wait $CPP_PID $NODE_PID
