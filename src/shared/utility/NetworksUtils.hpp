//
// Created by roussierenoa on 4/14/25.
//

#ifndef NETWORKSUTILS_HPP
#define NETWORKSUTILS_HPP
#include "communicationHeader.hpp"

Header_t generateHeader(unsigned char nbrPayload);
Payload_t generatePayload(unsigned char dataCount, unsigned char dataId);
#endif //NETWORKSUTILS_HPP
