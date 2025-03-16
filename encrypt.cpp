#include "encrypt.h"
#include "utils.h"
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>

std::string generateRSAKeyPair(std::string &publicKey, std::string &privateKey) {
    RSA *rsa = RSA_generate_key(2048, RSA_F4, nullptr, nullptr);

    BIO *pub = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPublicKey(pub, rsa);

    BIO *priv = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPrivateKey(priv, rsa, nullptr, nullptr, 0, nullptr, nullptr);

    size_t pub_len = BIO_pending(pub);
    size_t priv_len = BIO_pending(priv);

    char *pub_key = new char[pub_len + 1];
    char *priv_key = new char[priv_len + 1];

    BIO_read(pub, pub_key, pub_len);
    BIO_read(priv, priv_key, priv_len);

    pub_key[pub_len] = '\0';
    priv_key[priv_len] = '\0';

    publicKey = std::string(pub_key);
    privateKey = std::string(priv_key);

    delete[] pub_key;
    delete[] priv_key;

    RSA_free(rsa);
    BIO_free_all(pub);
    BIO_free_all(priv);

    return "RSA key pair generated!";
}

std::string encryptRSA(const std::string &message, const std::string &publicKey) {
    RSA *rsa = RSA_new();
    BIO *keybio = BIO_new_mem_buf(publicKey.c_str(), -1);
    PEM_read_bio_RSAPublicKey(keybio, &rsa, nullptr, nullptr);

    std::vector<unsigned char> encrypted(RSA_size(rsa));
    int result = RSA_public_encrypt(
        message.size(),
        reinterpret_cast<const unsigned char *>(message.c_str()),
        encrypted.data(),
        rsa,
        RSA_PKCS1_OAEP_PADDING
    );

    RSA_free(rsa);
    BIO_free_all(keybio);

    if (result == -1) {
        throw std::runtime_error("RSA encryption failed");
    }

    return base64_encode(encrypted.data(), result);
}

std::string decryptRSA(const std::string &encryptedMessage, const std::string &privateKey) {
    RSA *rsa = RSA_new();
    BIO *keybio = BIO_new_mem_buf(privateKey.c_str(), -1);
    PEM_read_bio_RSAPrivateKey(keybio, &rsa, nullptr, nullptr);

    std::vector<unsigned char> decrypted(RSA_size(rsa));
    std::string decoded = base64_decode(encryptedMessage);

    int result = RSA_private_decrypt(
        decoded.size(),
        reinterpret_cast<const unsigned char *>(decoded.c_str()),
        decrypted.data(),
        rsa,
        RSA_PKCS1_OAEP_PADDING
    );

    RSA_free(rsa);
    BIO_free_all(keybio);

    if (result == -1) {
        throw std::runtime_error("RSA decryption failed");
    }

    return std::string(reinterpret_cast<char *>(decrypted.data()), result);
}

std::string encryptAES(const std::string &message, const std::string &key) {
    AES_KEY encryptKey;
    AES_set_encrypt_key(reinterpret_cast<const unsigned char *>(key.c_str()), 128, &encryptKey);

    std::vector<unsigned char> encrypted(message.size() + AES_BLOCK_SIZE);
    unsigned char iv[AES_BLOCK_SIZE];
    RAND_bytes(iv, AES_BLOCK_SIZE);

    int encryptedSize = 0;
    AES_cfb128_encrypt(
        reinterpret_cast<const unsigned char *>(message.c_str()),
        encrypted.data(),
        message.size(),
        &encryptKey,
        iv,
        &encryptedSize,
        AES_ENCRYPT
    );

    return base64_encode(iv, AES_BLOCK_SIZE) + ":" + base64_encode(encrypted.data(), message.size());
}

std::string decryptAES(const std::string &encryptedMessage, const std::string &key) {
    AES_KEY decryptKey;
    AES_set_decrypt_key(reinterpret_cast<const unsigned char *>(key.c_str()), 128, &decryptKey);

    auto pos = encryptedMessage.find(':');
    if (pos == std::string::npos) throw std::runtime_error("Invalid AES message format");

    std::string iv_base64 = encryptedMessage.substr(0, pos);
    std::string encrypted_base64 = encryptedMessage.substr(pos + 1);

    std::string iv = base64_decode(iv_base64);
    std::string encrypted = base64_decode(encrypted_base64);

    std::vector<unsigned char> decrypted(encrypted.size());
    int decryptedSize = 0;

    AES_cfb128_encrypt(
        reinterpret_cast<const unsigned char *>(encrypted.c_str()),
        decrypted.data(),
        encrypted.size(),
        &decryptKey,
        reinterpret_cast<unsigned char *>(&iv[0]),
        &decryptedSize,
        AES_DECRYPT
    );

    return std::string(reinterpret_cast<char *>(decrypted.data()), decryptedSize);
}
