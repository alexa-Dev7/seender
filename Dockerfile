# Node.js build stage
FROM node:20 AS node-builder

# Set working directory
WORKDIR /app

# Copy the whole seender folder into /app
COPY . /app

# Install Node.js dependencies
RUN npm install --omit=dev

# C++ build stage
FROM gcc:latest AS cpp-builder

# Set working directory
WORKDIR /app

# Install C++ dependencies
RUN apt-get update && apt-get install -y cmake libssl-dev

# Copy everything from node-builder
COPY --from=node-builder /app /app

# Compile the C++ server
RUN cmake . && make

# Final stage
FROM ubuntu:latest

# Set working directory
WORKDIR /app

# Copy compiled app from cpp-builder stage
COPY --from=cpp-builder /app /app

# Expose port
EXPOSE 8080

# Run the app
CMD ["./sender"]
