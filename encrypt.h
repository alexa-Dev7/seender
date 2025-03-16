#ifndef ENCRYPT_H
#define ENCRYPT_H

#include <string>

// RSA Key generation (public & private keys)
std::string generateRSAKeyPair(std::string &publicKey, std::string &privateKey);

// RSA encryption and decryption
std::string encryptRSA(const std::string &message, const std::string &publicKey);
std::string decryptRSA(const std::string &encryptedMessage, const std::string &privateKey);

// AES encryption and decryption
std::string encryptAES(const std::string &message, const std::string &key);
std::string decryptAES(const std::string &encryptedMessage, const std::string &key);

#endif
