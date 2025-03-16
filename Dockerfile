ENV NODE_PORT=8081
ENV CPP_PORT=8080

# Use a lightweight C++ base image
FROM gcc:latest

# Set the working directory inside the container
WORKDIR /app

# Copy all files from the "sender" folder to the container
COPY sender/ .

# Install necessary packages
RUN apt-get update && apt-get install -y \
    cmake \
    libssl-dev \
    nodejs \
    npm \
    && rm -rf /var/lib/apt/lists/*

# Install Node.js dependencies (new integration)
RUN npm install express body-parser axios

# Compile the C++ project with CMake
RUN cmake . && make

# Expose port 8080 for the server
EXPOSE 8080

# Set the command to start the server and Node backend
CMD ["bash", "render-start.sh"]
