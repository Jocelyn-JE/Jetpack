//
// Created by roussierenoa on 4/14/25.
//


#include "NetworksUtils.hpp"

Header_t generateHeader(unsigned char nbrPayload)
{
	Header_t header{};
	header.magic1 = 42;
	header.magic2 = 42;
	header.nbrPayload = nbrPayload;
	return header;
}

Payload_t generatePayload(unsigned char dataCount, unsigned char dataId)
{
	Payload_t payload{};
	payload.dataCount = dataCount;
	payload.dataId = dataId;
	return payload;
}