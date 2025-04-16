//
// Created by roussierenoa on 4/14/25.
//

#include <sys/poll.h>

#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>

#include "communicationHeader.hpp"
#include "Exception.hpp"
#include "NetworksUtils.hpp"

#include "Socket.hpp"
#include "Player.hpp"
#include "Coin.hpp"
#include "Obstacle.hpp"
#include "Logger.hpp"

jetpack::Header_t generateHeader(unsigned char nbrPayload) {
    jetpack::Header_t header{};
    header.magic1 = 42;
    header.magic2 = 42;
    header.nbrPayload = nbrPayload;
    return header;
}

jetpack::Payload_t generatePayload(unsigned char dataCount,
    unsigned char dataId) {
    jetpack::Payload_t payload{};
    payload.dataCount = dataCount;
    payload.dataId = dataId;
    return payload;
}

int getPayloadSize(unsigned char dataId) {
    switch (dataId) {
        case jetpack::PayloadType_t::INT:
            return 4;
        case jetpack::PayloadType_t::SIZE_T:
            return 8;
        case jetpack::PayloadType_t::BOOL:
            return 1;
        case jetpack::PayloadType_t::DOUBLE:
            return 8;
        case jetpack::PayloadType_t::FLOAT:
            return 4;
        case jetpack::PayloadType_t::SHORT:
            return 2;
        case jetpack::PayloadType_t::PLAYER:
            return 39;
        case jetpack::PayloadType_t::NAME:
            return 20;
        case jetpack::PayloadType_t::COIN_POS:
            return 16;
        case jetpack::PayloadType_t::HAZARD_POS:
            return sizeof(obstacle_s);
        case jetpack::PayloadType_t::ENUM_COMMAND:
            return 1;
        case jetpack::PayloadType_t::GAMESPEED:
            return 4;
        case jetpack::PayloadType_t::PLAYER_INPUT:
            return 1;
        case jetpack::PayloadType_t::START:
            return 0;
        case jetpack::PayloadType_t::HEALTHCHECK:
            return 0;
        case jetpack::PayloadType_t::JETPACK_FORCE:
            return 4;
        case jetpack::PayloadType_t::VELOCITY_LIMITS:
            return 8;
        default:
            return 0;
    }
}


jetpack::Header_t getHeader(jetpack::Logger &logger, Socket &socket) {
    struct pollfd pfd;
    pfd.fd = socket.getSocketFd();
    pfd.events = POLLIN;
    int ret = poll(&pfd, 1, 1000);
    if (ret == -1) {
        logger.log("poll() failed: " + std::string(strerror(errno)));
        throw NetworkException("poll() failed");
    } else if (ret == 0) {
        logger.log("poll() timeout waiting for header");
        throw NetworkException("Timeout waiting for header");
    }
    std::vector<unsigned char> headerBuffer;
    headerBuffer.resize(2);
    headerBuffer = socket.readFromSocket(2);
    if (headerBuffer.empty() || headerBuffer ==
        std::vector<unsigned char>(2, 0)) {
        logger.log("Header Error or Server disconnected");
        throw HeaderException("Incomplete Header size = 0");
    }
    if (headerBuffer.size() < 2) {
        logger.log("Incomplete Header");
        throw HeaderException("Incomplete Header size = " +
            std::to_string(headerBuffer.size()));
    }
    logger.log("Packet received: ");
    std::stringstream ss;
    for (const auto& byte : headerBuffer) {
        ss << std::hex << std::uppercase << std::setw(2)
           << std::setfill('0') << static_cast<int>(byte) << " ";
        ss << std::dec;
    }
    logger.log(ss.str());
    jetpack::Header_t header {};
    uint16_t dataHeader = (static_cast<uint16_t>(headerBuffer[0]) << 8) |
                          static_cast<uint16_t>(headerBuffer[1]);
    header.rawData = ntohs(dataHeader);
    __bswap_64(header.rawData);;
    logger.log("Received: littleEndian " + std::to_string(header.rawData));
    logger.log("Received: " + std::to_string(dataHeader));
    logger.log("magic1: " + std::to_string(header.magic1));
    logger.log("magic2: " + std::to_string(header.magic2));
    logger.log("nbrPayload: " + std::to_string(header.nbrPayload));
    if (!(header.magic1 == 42 && header.magic2 == 42)) {
        throw NetworkException("Message not valid, no magic number");
    }
    return header;
}

jetpack::Payload_t getPayload(jetpack::Logger &logger, Socket &socket) {
    struct pollfd pfd;
    pfd.fd = socket.getSocketFd();
    pfd.events = POLLIN;

    int ret = poll(&pfd, 1, 1000);
    if (ret == -1) {
        logger.log("poll() failed: " + std::string(strerror(errno)));
        throw NetworkException("poll() failed");
    } else if (ret == 0) {
        logger.log("poll() timeout waiting for payload");
        throw NetworkException("Timeout waiting for payload");
    }
    std::vector<unsigned char> payloadBuffer = socket.readFromSocket(2);
    if (payloadBuffer.empty()) {
        logger.log("Payload Error or Server disconnected");
        throw PayloadException("Incomplete Payload size = 0");
    }
    if (payloadBuffer.size() < 2) {
        logger.log("Incomplete Payload");
        throw PayloadException("Incomplete Payload size = " +
            std::to_string(payloadBuffer.size()));
    }
    logger.log("Packet received: ");
    std::stringstream ss;
    for (const auto &byte : payloadBuffer) {
        ss << std::hex << std::uppercase << std::setw(2)
           << std::setfill('0') << static_cast<int>(byte) << " ";
        ss << std::dec;
    }
    logger.log(ss.str());
    jetpack::Payload_t payload = {};
    uint16_t dataPayload = (static_cast<uint8_t>(payloadBuffer[0]) << 8) |
                            static_cast<uint8_t>(payloadBuffer[1]);
    payload.rawData = ntohs(dataPayload);
    logger.log("Received: littleEndian " + std::to_string(payload.rawData));
    logger.log("Received: " + std::to_string(dataPayload));
    logger.log("data count: " + std::to_string(payload.dataCount));
    logger.log("data id: " + std::to_string(payload.dataId));
    return payload;
}
