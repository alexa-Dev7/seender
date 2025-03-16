# Stage 1: Build C++ server
FROM ubuntu:latest AS cpp-builder

# Install required dependencies
RUN apt-get update && apt-get install -y cmake g++ libssl-dev git

# Set working directory
WORKDIR /app

# Copy everything from the "seender" folder into the container
COPY seender /app

# Compile the C++ server
RUN cmake . && make

# Stage 2: Create the final runtime image
FROM ubuntu:latest

# Copy compiled server from build stage
COPY --from=cpp-builder /app /app
WORKDIR /app

# Ensure assets folder exists
RUN mkdir -p /app/assets

# Expose server port
EXPOSE 8080

# Run the server
CMD ["./sender"]
