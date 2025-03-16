#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Random token generator for secure sessions
std::string generateRandomToken(int length = 32);

// Bcrypt password hashing and verification
std::string hashPassword(const std::string &password);
bool verifyPassword(const std::string &password, const std::string &hashedPassword);

// JSON file reading and writing
json readJSON(const std::string &filename);
void writeJSON(const std::string &filename, const json &data);

// Base64 encoding and decoding (supporting encryption functions)
std::string base64_encode(const unsigned char *data, size_t length);
std::string base64_decode(const std::string &encoded);

#endif
