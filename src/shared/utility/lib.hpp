//
// Created by roussierenoa on 3/28/25.
//

#ifndef SRC_SHARED_UTILITY_LIB_HPP_
#define SRC_SHARED_UTILITY_LIB_HPP_

#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> splitSentence(std::string sen, char delimiter = '\n');
bool isNumber(const std::string &str);

#endif  // SRC_SHARED_UTILITY_LIB_HPP_
