#include "crypto.h"

string sha256(const string &str) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((const unsigned char *) str.c_str(), str.length(), hash);
    stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << hex << setw(2) << setfill('0') << (int) hash[i];
    }
    return ss.str();
}

vector<int> buildPositions(string pattern) {
    vector<int> positions(256, -1);
    for (int i = 0; i < pattern.length(); ++i) {
        positions[pattern[i]] = i;
    }
    return positions;
}


bool stringMatching(const string &text, const string &pattern) {
    int n = text.length();
    int m = pattern.length();
    for (int j = 0; j < n - m; ++j) {
        int i = 0;
        while (pattern[i] == text[i + j] && i < m) {
            ++i;
        }
        if (i == m) {
            return true;
        }
    }
    return false;
}

bool stringStart(const string &text, const string &pattern){
    for (int i = 0; i < pattern.size(); ++i) {
        if(text[i] != pattern[i]) return false;
    }
    return true;
}





