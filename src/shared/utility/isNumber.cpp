//
// Created by roussierenoa on 3/28/25.
//

#include <string>

#include "lib.hpp"

bool isNumber(const std::string &str) {
    if (str.empty()) return false;
    for (auto c : str) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    return true;
}
