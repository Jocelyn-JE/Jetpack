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
#include <iostream>
#include <typeinfo>
#include <vector>

#include "CommunicationHeader.hpp"

namespace jetpack::server {

class Packet {
 public:
    Packet() = delete;
    explicit Packet(uint8_t nbrPayload);
    void addPayloadHeader(uint16_t dataCount, uint8_t dataId);
    void addData(uint64_t data);
    void addData(uint32_t data);
    void addData(uint16_t data);
    void addData(uint8_t data);
    void addData(float data);
    void addData(double data);
    void addData(char *data, size_t size);
    template <typename T>
    void addData(T data) {
        // std::cout << "Size of data: " << sizeof(T)
        //           << ", Name of data: " << typeid(T).name() << std::endl;
        for (int i = 0; i < static_cast<int>(sizeof(T)); i++) {
            this->_packet.push_back(static_cast<uint8_t>(
                (data >> (8 * static_cast<T>(sizeof(T) - 1 - i))) & 0xFF));
        }
    }
    void add(uint64_t data, PayloadType_t type);
    void add(uint32_t data, PayloadType_t type);
    void add(uint16_t data, PayloadType_t type);
    void add(uint8_t data, PayloadType_t type);
    void add(std::vector<uint64_t> data, PayloadType_t type);
    void add(std::vector<uint32_t> data, PayloadType_t type);
    void add(std::vector<uint16_t> data, PayloadType_t type);
    void add(std::vector<uint8_t> data, PayloadType_t type);
    template <typename T>
    void add(T data, PayloadType_t type) {
        this->addPayloadHeader(1, type);
        this->addData(data);
    }
    template <typename T>
    void add(std::vector<T> data, PayloadType_t type) {
        this->addPayloadHeader(data.size(), type);
        for (const auto &item : data) {
            this->addData(item);
        }
    }
    const std::vector<uint8_t> &getPacket() const;

 private:
    static Payload_t createPayloadHeader(uint16_t dataCount, uint8_t dataId);
    static Header_t createPacketHeader(uint8_t nbrPayload);
    std::vector<uint8_t> _packet;
};

}  // namespace jetpack::server

#endif  // SRC_SERVER_PACKET_HPP_
