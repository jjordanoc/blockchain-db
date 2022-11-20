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

string sha256(const string &str);

#endif//PROYECTO_SHA256_CRYPTO_H
