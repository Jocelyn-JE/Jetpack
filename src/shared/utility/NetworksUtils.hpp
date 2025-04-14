//
// Created by roussierenoa on 4/14/25.
//

#ifndef SRC_SHARED_UTILITY_NETWORKSUTILS_HPP_
#define SRC_SHARED_UTILITY_NETWORKSUTILS_HPP_
#include "communicationHeader.hpp"

Header_t generateHeader(unsigned char nbrPayload);
Payload_t generatePayload(unsigned char dataCount, unsigned char dataId);
#endif  // SRC_SHARED_UTILITY_NETWORKSUTILS_HPP_
