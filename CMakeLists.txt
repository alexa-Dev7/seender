cmake_minimum_required(VERSION 3.10)
project(SenderChatApp)

set(CMAKE_CXX_STANDARD 17)

# Include OpenSSL
find_package(OpenSSL REQUIRED)

# Define source files
set(SOURCE_FILES server.cpp encrypt.cpp utils.cpp)

# Define the executable
add_executable(sender ${SOURCE_FILES})

# Link OpenSSL
target_link_libraries(sender OpenSSL::Crypto)

# Copy assets folder into the app directory
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION /app)

# Add definitions for encryption and settings
add_definitions(-DENABLE_ENCRYPTION -DUSE_BCRYPT -DSHORT_POLLING=1)
