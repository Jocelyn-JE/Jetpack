//
// Created by roussierenoa on 4/3/25.
//

#ifndef COMMUNICATIONHEADER_HPP
#define COMMUNICATIONHEADER_HPP
#include <arpa/inet.h>

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
