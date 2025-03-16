# Minimum CMake version requirement
cmake_minimum_required(VERSION 3.10)

# Project name and C++ standard setup
project(SenderChatApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Output executable setup
add_executable(server 
    server.cpp 
    encrypt.cpp 
    utils.cpp
)

# Include headers
target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR})

# Find and link bcrypt (crypto library for hashing)
find_package(OpenSSL REQUIRED)
target_link_libraries(server OpenSSL::Crypto)

# Enable optimizations for production
set(CMAKE_BUILD_TYPE Release)
add_compile_options(-O2 -Wall -Wextra -Werror)

# Ensure the assets directory is included in the output folder
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

# Output messages for the build process
message(STATUS "Building SenderChatApp with C++17")
message(STATUS "Encryption: RSA & AES with OpenSSL")
message(STATUS "Bcrypt password handling enabled")
message(STATUS "Short polling every 1 second enabled")
