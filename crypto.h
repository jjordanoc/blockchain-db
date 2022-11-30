//
// Created by renato on 10/5/22.
//

#ifndef PROYECTO_SHA256_CRYPTO_H
#define PROYECTO_SHA256_CRYPTO_H

#include <iomanip>
#include <sstream>
#include <string>
#include <chrono>
#include <vector>
using namespace std;

#include <openssl/sha.h>

string sha256(const string &str);

template<typename Return>
struct TimedResult {
    Return result;
    double duration;
    TimedResult(Return &_result, double &_duration) : result(_result), duration(_duration) {}
};

template<>
struct TimedResult<void> {
    double duration;
    TimedResult(double &_duration) : duration(_duration) {}
};

template<typename Return, typename Fun, typename ...Args>
TimedResult<Return> time_function(Fun &function, Args... args) {
    auto start = std::chrono::steady_clock::now();
    Return result = function(args...);
    auto end = std::chrono::steady_clock::now();
    double duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
    return {result, duration};
}

template<typename Fun, typename ...Args>
TimedResult<void> time_function(Fun &function, Args... args) {
    auto start = std::chrono::steady_clock::now();
    function(args...);
    auto end = std::chrono::steady_clock::now();
    double duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
    return {duration};
}

vector<int> buildPositions(string pattern);

bool stringMatching(const string &text, const string &pattern);

bool stringStart(const string &text, const string &pattern);

#endif//PROYECTO_SHA256_CRYPTO_H
