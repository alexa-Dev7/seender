# Stage 1: Build the C++ server
FROM ubuntu:latest AS cpp-builder

# Install required dependencies
RUN apt-get update && apt-get install -y cmake g++ libssl-dev git

# Set the working directory
WORKDIR /app

# Copy everything from the "seender" folder into /app
COPY . .

# Compile the C++ server
RUN cmake . && make

# Stage 2: Final runtime image
FROM ubuntu:latest

# Copy the built app from the builder stage
COPY --from=cpp-builder /app /app

# Ensure assets folder exists
RUN mkdir -p /app/assets

# Expose the server port
EXPOSE 8080

# Set the command to run the server
CMD ["./sender"]
