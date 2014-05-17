#pragma once

#include <string>
#include <functional>
#include <algorithm>

// http://stackoverflow.com/a/217605
static inline std::string& ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

static inline std::string& rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// Modifies the string
static inline std::string& trim(std::string& s) {
    return ltrim(rtrim(s));
}

// Leaves original intact and returns a trimmed version
static inline std::string trim(const std::string& s) {
    std::string result(s);
    trim(result);
    return result;
}
