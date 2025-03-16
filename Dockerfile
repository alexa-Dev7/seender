# Node.js build stage
FROM node:20 AS node-builder

# Set working directory
WORKDIR /app

# Copy Node backend files
COPY index.js package*.json .env ./

# Verify package.json is copied correctly
RUN ls -la /app

# Install Node.js dependencies safely
RUN npm install --production

# C++ build stage
FROM gcc:latest AS cpp-builder

# Set working directory
WORKDIR /app

# Copy C++ source files
COPY server.cpp encrypt.cpp encrypt.h utils.cpp utils.h CMakeLists.txt ./

# Install build tools and libraries
RUN apt-get update && apt-get install -y cmake libssl-dev

# Build C++ server
RUN cmake . && make

# Final production image (lightweight Ubuntu)
FROM ubuntu:latest

# Set working directory
WORKDIR /app

# Copy Node build from node-builder stage
COPY --from=node-builder /app /app

# Copy compiled C++ server from cpp-builder stage
COPY --from=cpp-builder /app/server /app

# Copy PHP frontend files and assets
COPY *.php ./
COPY assets/ ./assets

# Install PHP and system dependencies
RUN apt-get update && apt-get install -y php-cli

# Set environment variables for Node and C++ ports
ENV NODE_PORT=8081
ENV CPP_PORT=8080

# Expose required ports
EXPOSE 8081 8080

# Copy and prepare the startup script
COPY render-start.sh .
RUN chmod +x render-start.sh

# Ensure the container runs both servers
CMD ["./render-start.sh"]
