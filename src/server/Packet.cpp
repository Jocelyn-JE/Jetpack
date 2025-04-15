/*
** EPITECH PROJECT, 2025
** Jetpack
** File description:
** Packet
*/

#include "Packet.hpp"

#include <cstring>
#include <iomanip>
#include <iostream>
#include <vector>

namespace jetpack::server {

Header_t Packet::createPacketHeader(uint8_t nbrPayload) {
    Header_t header;
    header.magic1 = 42;
    header.magic2 = 42;
    header.nbrPayload = nbrPayload;
    return header;
}

Payload_t Packet::createPayloadHeader(uint16_t dataCount, uint8_t dataId) {
    Payload_t payload;
    payload.dataCount = dataCount;
    payload.dataId = dataId;
    return payload;
}

Packet::Packet(uint8_t nbrPayload) {
    uint16_t headerBigEndian = htons(createPacketHeader(nbrPayload).rawData);
    this->_packet.push_back(static_cast<uint8_t>(headerBigEndian >> 8));
    this->_packet.push_back(static_cast<uint8_t>(headerBigEndian & 0xFF));
}

void Packet::addPayloadHeader(uint16_t dataCount, uint8_t dataId) {
    uint16_t payload = htons(createPayloadHeader(dataCount, dataId).rawData);
    this->_packet.push_back(static_cast<uint8_t>(payload >> 8));
    this->_packet.push_back(static_cast<uint8_t>(payload & 0xFF));
}

void Packet::addData(uint32_t data) {
    uint32_t bigEndianData = htonl(data);
    this->_packet.push_back(static_cast<uint8_t>(bigEndianData >> 24));
    this->_packet.push_back(static_cast<uint8_t>(bigEndianData >> 16));
    this->_packet.push_back(static_cast<uint8_t>(bigEndianData >> 8));
    this->_packet.push_back(static_cast<uint8_t>(bigEndianData & 0xFF));
}

void Packet::addData(uint16_t data) {
    uint16_t bigEndianData = htons(data);
    this->_packet.push_back(static_cast<uint8_t>(bigEndianData >> 8));
    this->_packet.push_back(static_cast<uint8_t>(bigEndianData & 0xFF));
}

void Packet::addData(uint8_t data) {
    this->_packet.push_back(static_cast<uint8_t>(data & 0xFF));
}

void Packet::addData(float data) {
    uint32_t rawData;
    uint32_t bigEndianData;
    std::memcpy(&rawData, &data, sizeof(data));
    bigEndianData = htonl(rawData);
    this->_packet.push_back(static_cast<uint8_t>(bigEndianData >> 24));
    this->_packet.push_back(static_cast<uint8_t>(bigEndianData >> 16));
    this->_packet.push_back(static_cast<uint8_t>(bigEndianData >> 8));
    this->_packet.push_back(static_cast<uint8_t>(bigEndianData & 0xFF));
}

void Packet::addData(char* data, size_t size) {
    for (size_t i = 0; i <= size; i++) this->addData(data[i]);
}

void Packet::add(uint32_t data, PayloadType_t type) {
    this->addPayloadHeader(1, type);
    this->addData(data);
}

void Packet::add(uint16_t data, PayloadType_t type) {
    this->addPayloadHeader(1, type);
    this->addData(data);
}

void Packet::add(uint8_t data, PayloadType_t type) {
    this->addPayloadHeader(1, type);
    this->addData(data);
}

void Packet::add(std::vector<uint32_t> dataList, PayloadType_t type) {
    this->addPayloadHeader(dataList.size(), type);
    for (const auto& data : dataList) this->addData(data);
}

void Packet::add(std::vector<uint16_t> dataList, PayloadType_t type) {
    this->addPayloadHeader(dataList.size(), type);
    for (const auto& data : dataList) this->addData(data);
}

void Packet::add(std::vector<uint8_t> dataList, PayloadType_t type) {
    this->addPayloadHeader(1, type);
    this->addPayloadHeader(dataList.size(), type);
    for (const auto& data : dataList) this->addData(data);
}

const std::vector<uint8_t>& Packet::getPacket() const {
    std::cerr << "Packet created: " << std::endl;
    for (const auto& byte : this->_packet) {
        std::cerr << std::hex << std::uppercase << std::setw(2)
                  << std::setfill('0') << static_cast<int>(byte) << " ";
    }
    std::cerr << std::dec << std::endl;
    return this->_packet;
}

}  // namespace jetpack::server
