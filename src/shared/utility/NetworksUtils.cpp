//
// Created by roussierenoa on 4/14/25.
//

#include "NetworksUtils.hpp"

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
