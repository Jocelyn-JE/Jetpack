/*
** EPITECH PROJECT, 2025
** Jetpack
** File description:
** Packet
*/

#include "Packet.hpp"

#include <iomanip>
#include <iostream>
#include <vector>

namespace jetpack::server {

Header_t Packet::createPacketHeader(u_int8_t nbrPayload) {
    Header_t header;
    header.magic1 = 42;
    header.magic2 = 42;
    header.nbrPayload = nbrPayload;
    return header;
}

Payload_t Packet::createPayloadHeader(u_int16_t dataCount, u_int8_t dataId) {
    Payload_t payload;
    payload.dataCount = dataCount;
    payload.dataId = dataId;
    return payload;
}

Packet::Packet(uint8_t nbrPayload) {
    uint16_t headerBigEndian = htons(createPacketHeader(nbrPayload).rawData);
    _packet.push_back(static_cast<uint8_t>(headerBigEndian >> 8));
    _packet.push_back(static_cast<uint8_t>(headerBigEndian & 0xFF));
}

void Packet::addPayloadHeader(uint8_t dataCount, uint8_t dataId) {
    uint16_t payload = htons(createPayloadHeader(dataCount, dataId).rawData);
    _packet.push_back(static_cast<uint8_t>(payload >> 8));
    _packet.push_back(static_cast<uint8_t>(payload & 0xFF));
}

void Packet::addData(uint32_t data) {
    uint32_t bigEndianData = htonl(data);
    _packet.push_back(static_cast<uint8_t>(bigEndianData >> 24));
    _packet.push_back(static_cast<uint8_t>(bigEndianData >> 16));
    _packet.push_back(static_cast<uint8_t>(bigEndianData >> 8));
    _packet.push_back(static_cast<uint8_t>(bigEndianData & 0xFF));
}

void Packet::addData(uint16_t data) {
    uint16_t bigEndianData = htons(data);
    _packet.push_back(static_cast<uint8_t>(bigEndianData >> 8));
    _packet.push_back(static_cast<uint8_t>(bigEndianData & 0xFF));
}

void Packet::addData(uint8_t data) {
    _packet.push_back(static_cast<uint8_t>(data & 0xFF));
}

void Packet::add(uint32_t data, PayloadType_t type) {
    addPayloadHeader(1, type);
    addData(data);
}

void Packet::add(uint16_t data, PayloadType_t type) {
    addPayloadHeader(1, type);
    addData(data);
}

void Packet::add(uint8_t data, PayloadType_t type) {
    addPayloadHeader(1, type);
    addData(data);
}

void Packet::add(std::vector<uint32_t> data, PayloadType_t type) {
    addPayloadHeader(data.size(), type);
    for (const auto& d : data) {
        addData(d);
    }
}

void Packet::add(std::vector<uint16_t> data, PayloadType_t type) {
    addPayloadHeader(data.size(), type);
    for (const auto& d : data) {
        addData(d);
    }
}

void Packet::add(std::vector<uint8_t> data, PayloadType_t type) {
    addPayloadHeader(1, type);
    addPayloadHeader(data.size(), type);
    for (const auto& d : data) {
        addData(d);
    }
}

const std::vector<uint8_t>& Packet::getPacket() const { return _packet; }

}  // namespace jetpack::server
