//
// Created by renato on 10/5/22.
//

#ifndef PROYECTO_SHA256_CRYPTO_H
#define PROYECTO_SHA256_CRYPTO_H

#include <iomanip>
#include <sstream>
#include <string>
using namespace std;

#include <openssl/sha.h>

string sha256(const string &str) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((const unsigned char *) str.c_str(), str.length(), hash);
    stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << hex << setw(2) << setfill('0') << (int) hash[i];
    }
    return ss.str();
}

#endif//PROYECTO_SHA256_CRYPTO_H
