//
// Created by roussierenoa on 4/14/25.
//

#include "NetworksUtils.hpp"
#include "Player.hpp"
#include "Coin.hpp"
#include "Obstacle.hpp"

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
            return 4;
        case jetpack::PayloadType_t::BOOL:
            return 1;
        case jetpack::PayloadType_t::DOUBLE:
            return 8;
        case jetpack::PayloadType_t::FLOAT:
            return 4;
        case jetpack::PayloadType_t::SHORT:
            return 2;
        case jetpack::PayloadType_t::PLAYER:
            return sizeof(player);
        case jetpack::PayloadType_t::NAME:
            return 20;
        case jetpack::PayloadType_t::COIN_POS:
            return sizeof(coinsPos_s);
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

