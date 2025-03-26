/*
** EPITECH PROJECT, 2025
** Jetpack
** File description:
** ToBytes
*/

#ifndef SERVER_SRC_SERVER_TOBYTES_HPP_
    #define SERVER_SRC_SERVER_TOBYTES_HPP_

    #include <vector>
    #include <cstdint>

namespace jetpack {
template <typename T>
std::vector<uint8_t> toBytes(const T &data) {
    std::vector<uint8_t> bytes(sizeof(T));

    std::memcpy(bytes.data(), reinterpret_cast<void*>(&data), sizeof(T));
    return bytes;
}
}  // namespace jetpack

#endif  // SERVER_SRC_SERVER_TOBYTES_HPP_
