//
// Created by roussierenoa on 4/3/25.
//

#ifndef COMMUNICATIONHEADER_HPP
#define COMMUNICATIONHEADER_HPP
#include <arpa/inet.h>

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
		unsigned char magic1: 6;
		unsigned char magic2: 6;
		unsigned char nbrPayload: 4;
	};

	unsigned int rawData: 16;
};

union Payload_t {
	struct {
		unsigned short dataCount: 10;
		unsigned char dataId: 6;
	};

	unsigned int rawData: 16;
};

#pragma pack(pop)

#endif //COMMUNICATIONHEADER_HPP
