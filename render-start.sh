#!/bin/bash

export NODE_PORT=8081
export CPP_PORT=8080

# Start both servers
node index.js & ./server

#!/bin/bash

# Ensure the script fails on any error
set -e

# Show logs for visibility
echo "Starting SenderChatApp..."

# Launch the C++ server backend
echo "Launching C++ server backend..."
./server &

# Launch the Node.js backend alongside it
echo "Launching Node.js backend..."
node index.js &

# Keep the container running
wait
