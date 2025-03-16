# Use multi-stage build to keep it clean and fast
FROM node:20 AS node-builder

# Set working directory
WORKDIR /app

# Copy Node.js files
COPY index.js package*.json .env ./

# Install Node dependencies
RUN npm install

# Build C++ backend
FROM gcc:latest AS cpp-builder

# Set working directory
WORKDIR /app

# Copy C++ files
COPY server.cpp encrypt.cpp encrypt.h utils.cpp utils.h CMakeLists.txt ./

# Build C++ server
RUN apt-get update && apt-get install -y cmake
RUN cmake . && make

# Final production image
FROM ubuntu:latest

# Set working directory
WORKDIR /app

# Copy from previous stages
COPY --from=node-builder /app /app
COPY --from=cpp-builder /app/server /app

# Copy PHP and assets
COPY *.php ./  
COPY assets/ ./assets

# Install PHP and other dependencies
RUN apt-get update && apt-get install -y php-cli

# Expose ports
ENV NODE_PORT=8081
ENV CPP_PORT=8080
EXPOSE 8081 8080

# Set the startup script
COPY render-start.sh .
RUN chmod +x render-start.sh

CMD ["./render-start.sh"]
