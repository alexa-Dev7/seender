# -------------------------------
# Node.js Build Stage
# -------------------------------
FROM node:20 AS node-builder

# Set working directory
WORKDIR /app

# Copy Node.js files properly
COPY index.js package*.json .env ./

# Verify package.json exists (for debugging)
RUN ls -la /app

# Install Node.js dependencies safely
RUN npm ci --only=production

# -------------------------------
# C++ Build Stage
# -------------------------------
FROM gcc:latest AS cpp-builder

# Set working directory
WORKDIR /app

# Copy C++ source files
COPY server.cpp encrypt.cpp encrypt.h utils.cpp utils.h CMakeLists.txt ./

# Install required build tools and OpenSSL for encryption
RUN apt-get update && apt-get install -y cmake libssl-dev

# Compile C++ server
RUN cmake . && make

# -------------------------------
# Final Production Image
# -------------------------------
FROM ubuntu:latest

# Set working directory
WORKDIR /app

# Copy Node.js build from node-builder stage
COPY --from=node-builder /app /app

# Copy C++ server build from cpp-builder stage
COPY --from=cpp-builder /app/server /app

# Copy PHP frontend files and assets
COPY *.php ./
COPY assets/ ./assets

# Install PHP CLI and system dependencies
RUN apt-get update && apt-get install -y php-cli

# Set environment variables for Node.js and C++ ports
ENV NODE_PORT=8081
ENV CPP_PORT=8080

# Expose required ports
EXPOSE 8081 8080

# Copy and prepare the startup script
COPY render-start.sh .
RUN chmod +x render-start.sh

# Ensure the container runs both servers (Node + C++)
CMD ["./render-start.sh"]
