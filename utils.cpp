#include "utils.h"
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include <bcrypt.h>

using json = nlohmann::json;

// Generate a random string for session tokens
std::string generateRandomToken(int length) {
    std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::string token;
    for (int i = 0; i < length; ++i) {
        token += chars[rand() % chars.size()];
    }
    return token;
}

// Hash passwords using bcrypt
std::string hashPassword(const std::string &password) {
    char salt[BCRYPT_HASHSIZE];
    if (bcrypt_gensalt(12, salt) != 0) {
        throw std::runtime_error("Failed to generate salt");
    }

    char hashedPassword[BCRYPT_HASHSIZE];
    if (bcrypt_hashpw(password.c_str(), salt, hashedPassword) != 0) {
        throw std::runtime_error("Failed to hash password");
    }

    return std::string(hashedPassword);
}

// Verify bcrypt password hashes
bool verifyPassword(const std::string &password, const std::string &hashedPassword) {
    return bcrypt_checkpw(password.c_str(), hashedPassword.c_str()) == 0;
}

// Read JSON from file
json readJSON(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    json data;
    file >> data;
    file.close();
    return data;
}

// Write JSON to file
void writeJSON(const std::string &filename, const json &data) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to write to file: " + filename);
    }

    file << data.dump(4);
    file.close();
}

// Base64 encoding
std::string base64_encode(const unsigned char *data, size_t length) {
    static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string encoded;

    for (size_t i = 0; i < length; i += 3) {
        int val = (data[i] << 16) + ((i + 1 < length ? data[i + 1] : 0) << 8) + (i + 2 < length ? data[i + 2] : 0);
        encoded.push_back(base64_chars[(val >> 18) & 0x3F]);
        encoded.push_back(base64_chars[(val >> 12) & 0x3F]);
        encoded.push_back((i + 1 < length) ? base64_chars[(val >> 6) & 0x3F] : '=');
        encoded.push_back((i + 2 < length) ? base64_chars[val & 0x3F] : '=');
    }

    return encoded;
}

// Base64 decoding
std::string base64_decode(const std::string &encoded) {
    static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string decoded;
    std::vector<int> decode_table(256, -1);

    for (int i = 0; i < 64; ++i) decode_table[base64_chars[i]] = i;

    int val = 0, bits = -8;
    for (char c : encoded) {
        if (decode_table[c] == -1) break;

        val = (val << 6) + decode_table[c];
        bits += 6;

        if (bits >= 0) {
            decoded.push_back(char((val >> bits) & 0xFF));
            bits -= 8;
        }
    }

    return decoded;
}
