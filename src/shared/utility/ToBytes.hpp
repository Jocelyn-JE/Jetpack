/*
** EPITECH PROJECT, 2025
** Jetpack
** File description:
** ToBytes
*/

#ifndef SRC_SHARED_UTILITY_TOBYTES_HPP_
#define SRC_SHARED_UTILITY_TOBYTES_HPP_

#include <cstdint>
#include <cstring>
#include <vector>

namespace jetpack {
template <typename T>
std::vector<uint8_t> toBytes(const T &data) {
    std::vector<uint8_t> bytes(sizeof(T));

    std::memcpy(bytes.data(), reinterpret_cast<void *>(&data), sizeof(T));
    return bytes;
}
}  // namespace jetpack

#endif  // SRC_SHARED_UTILITY_TOBYTES_HPP_
