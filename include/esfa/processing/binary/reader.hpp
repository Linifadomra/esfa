#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "esfa/processing/bit/converter.hpp"
#include "esfa/processing/stream/stream.hpp"

namespace esfa::binary {

class Reader
{
public:
	Reader(esfa::stream::Stream* nStream);
	Reader(std::shared_ptr<esfa::stream::Stream> nStream);

	void Close();

	void SetEndianness(esfa::bit::Endianness endianness);
	esfa::bit::Endianness GetEndianness() const;

	void Seek(uint32_t offset, esfa::stream::SeekOffsetType seekType);
	uint64_t GetBaseAddress();

	void Read(int32_t length);
	void Read(char* buffer, int32_t length);
	char ReadChar();
	int8_t ReadByte();
	int16_t ReadInt16();
	int32_t ReadInt32();
	uint8_t ReadUByte();
	uint16_t ReadUInt16();
	uint32_t ReadUInt32();
	uint64_t ReadUInt64();
	float ReadSingle();
	double ReadDouble();
	std::string ReadString();

protected:
	std::shared_ptr<esfa::stream::Stream> stream;
	esfa::bit::Endianness endianness = esfa::bit::Endianness::Native;
};

}
