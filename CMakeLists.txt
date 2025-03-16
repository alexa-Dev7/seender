cmake_minimum_required(VERSION 3.10)
project(SenderChatApp)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)

# Include directories for uWebSockets and OpenSSL
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

# Define source files
set(SOURCES
    server.cpp
    encrypt.cpp
    utils.cpp
)

# Add the executable
add_executable(sender ${SOURCES})

# Link against uWebSockets, OpenSSL, and pthread
target_link_libraries(sender uWS ssl crypto pthread)

# Ensure assets folder is copied
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

# Optional: output the binary to /app folder
set_target_properties(sender PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "/app"
)

# Display build info
message(STATUS "Building SenderChatApp with C++17")
message(STATUS "Encryption: RSA & AES with OpenSSL")
message(STATUS "Bcrypt password handling enabled")
message(STATUS "Short polling every 1 second enabled")
