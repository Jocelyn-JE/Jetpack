//
// Created by roussierenoa on 4/3/25.
//

#ifndef SRC_SHARED_UTILITY_COMMUNICATIONHEADER_HPP_
#define SRC_SHARED_UTILITY_COMMUNICATIONHEADER_HPP_
#include <arpa/inet.h>

namespace jetpack {
enum PayloadType_t {
    INT = 1,
    SIZE_T = 2,
    BOOL = 3,
    DOUBLE = 4,
    FLOAT = 5,
    SHORT = 6,
    PLAYER = 7,
    NAME = 8,
    COIN_POS = 9,
    HAZARD_POS = 10,
    ENUM_COMMAND = 11,
    GAMESPEED = 12,
    PLAYER_INPUT = 13,
    START = 14,
    HEALTHCHECK = 15,
    JETPACK_FORCE = 16,
    VELOCITY_LIMITS = 17,
};

#pragma pack(push, 1)

union Header_t {
    struct {
        u_int16_t nbrPayload : 4;
        u_int16_t magic2     : 6;
        u_int16_t magic1     : 6;
    };

    u_int16_t rawData;
};

union Payload_t {
    struct {
        u_int8_t dataId : 6;
        u_int16_t dataCount : 10;
    };

    u_int16_t rawData : 16;
};
};  // namespace jetpack

#pragma pack(pop)

#endif  // SRC_SHARED_UTILITY_COMMUNICATIONHEADER_HPP_
