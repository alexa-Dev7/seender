# Use a multi-stage build for Node.js and C++
FROM node:20 AS node-builder

# Set working directory
WORKDIR /app

# Copy package files and install dependencies
COPY seender/package*.json ./
RUN npm install --omit=dev

# Copy all project files (including assets)
COPY seender /app

# C++ Build Stage
FROM gcc:latest AS cpp-builder

# Set working directory
WORKDIR /app

# Install C++ dependencies
RUN apt-get update && apt-get install -y cmake libssl-dev

# Copy source code and assets from the previous stage
COPY --from=node-builder /app /app

# Compile the C++ server
RUN cmake . && make

# Final Stage
FROM ubuntu:latest

# Set working directory
WORKDIR /app

# Copy compiled app from the C++ build stage
COPY --from=cpp-builder /app /app

# Expose port
EXPOSE 8080

# Run the app
CMD ["./sender"]
