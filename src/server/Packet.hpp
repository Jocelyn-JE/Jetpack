/*
** EPITECH PROJECT, 2025
** Jetpack
** File description:
** Packet
*/

#ifndef SRC_SERVER_PACKET_HPP_
#define SRC_SERVER_PACKET_HPP_

#include <netinet/in.h>

#include <cstdint>
#include <vector>

#include "CommunicationHeader.hpp"

namespace jetpack::server {

class Packet {
 public:
    Packet() = delete;
    explicit Packet(uint8_t nbrPayload);
    void addPayloadHeader(uint16_t dataCount, uint8_t dataId);
    void addData(uint32_t data);
    void addData(uint16_t data);
    void addData(uint8_t data);
    void add(uint32_t data, PayloadType_t type);
    void add(uint16_t data, PayloadType_t type);
    void add(uint8_t data, PayloadType_t type);
    void add(std::vector<uint32_t> data, PayloadType_t type);
    void add(std::vector<uint16_t> data, PayloadType_t type);
    void add(std::vector<uint8_t> data, PayloadType_t type);
    const std::vector<uint8_t> &getPacket() const;

 private:
    Payload_t createPayloadHeader(uint16_t dataCount, uint8_t dataId);
    Header_t createPacketHeader(uint8_t nbrPayload);
    std::vector<uint8_t> _packet;
};

}  // namespace jetpack::server

#endif  // SRC_SERVER_PACKET_HPP_
