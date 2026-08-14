#pragma once

#include <array>
#include <memory>
#include <string>
#include <vector>
#include <cstdint>

#include "esfa/processing/bit/converter.hpp"
#include "esfa/processing/stream/stream.hpp"

namespace esfa::binary {

class Writer
{
public:
	Writer(esfa::stream::Stream* nStream);
	Writer(std::shared_ptr<esfa::stream::Stream> nStream);

	void SetEndianness(esfa::bit::Endianness endianness);
	esfa::bit::Endianness GetEndianness() const;

	std::shared_ptr<esfa::stream::Stream> GetStream();
	uint64_t GetBaseAddress();
	uint64_t GetLength();
	void Seek(int64_t offset, esfa::stream::SeekOffsetType seekType);
	void Close();

	void Write(int8_t value);
	void Write(uint8_t value);
	void Write(int16_t value);
	void Write(uint16_t value);
	void Write(int32_t value);
	void Write(int32_t valueA, int32_t valueB);
	void Write(uint32_t value);
	void Write(int64_t value);
	void Write(uint64_t value);
	void Write(float value);
	void Write(double value);
	void Write(const std::string& str);
	void Write(char* srcBuffer, size_t length);

protected:
	std::shared_ptr<esfa::stream::Stream> stream;
	esfa::bit::Endianness endianness = esfa::bit::Endianness::Native;
};

}
