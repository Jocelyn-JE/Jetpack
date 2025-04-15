//
// Created by roussierenoa on 4/14/25.
//

#ifndef SRC_SHARED_UTILITY_NETWORKSUTILS_HPP_
#define SRC_SHARED_UTILITY_NETWORKSUTILS_HPP_

#include "CommunicationHeader.hpp"
#include "Logger.hpp"
#include "Socket.hpp"

jetpack::Header_t generateHeader(unsigned char nbrPayload);
jetpack::Payload_t generatePayload(unsigned char dataCount,
                                   unsigned char dataId);
int getPayloadSize(unsigned char dataId);
jetpack::Header_t getHeader(jetpack::Logger &logger, Socket &socket);
jetpack::Payload_t getPayload(jetpack::Logger &logger, Socket &socket);
#endif  // SRC_SHARED_UTILITY_NETWORKSUTILS_HPP_
