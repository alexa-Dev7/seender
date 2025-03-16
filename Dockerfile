# Node.js build stage
FROM node:20 AS node-builder

# Set working directory
WORKDIR /app

# Copy Node files
COPY index.js package*.json .env ./  

# Ensure package-lock.json exists to fix npm error
RUN npm install --only=production

# C++ build stage
FROM gcc:latest AS cpp-builder

# Set working directory
WORKDIR /app

# Copy C++ source files
COPY server.cpp encrypt.cpp encrypt.h utils.cpp utils.h CMakeLists.txt ./

# Install CMake and build C++ backend
RUN apt-get update && apt-get install -y cmake libssl-dev
RUN cmake . && make

# Final production image
FROM ubuntu:latest

# Set working directory
WORKDIR /app

# Copy Node and C++ builds
COPY --from=node-builder /app /app
COPY --from=cpp-builder /app/server /app

# Copy PHP frontend and assets
COPY *.php ./
COPY assets/ ./assets

# Install PHP and dependencies
RUN apt-get update && apt-get install -y php-cli

# Set environment variables for ports
ENV NODE_PORT=8081
ENV CPP_PORT=8080
EXPOSE 8081 8080

# Copy startup script
COPY render-start.sh .  
RUN chmod +x render-start.sh

# Launch both servers
CMD ["./render-start.sh"]
