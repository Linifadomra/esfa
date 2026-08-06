#include <cmath>
#include <stdexcept>

#include "esfa/processing/binary/reader.hpp"
#include "esfa/processing/stream/stream.hpp"

namespace esfa::binary {

Reader::Reader(esfa::stream::Stream* nStream)
{
	stream.reset(nStream);
}

Reader::Reader(std::shared_ptr<esfa::stream::Stream> nStream)
{
	stream = nStream;
}

void Reader::Close()
{
	stream->Close();
}

void Reader::SetEndianness(esfa::bit::Endianness endianness)
{
	this->endianness = endianness;
}

esfa::bit::Endianness Reader::GetEndianness() const
{
	return endianness;
}

void Reader::Seek(uint32_t offset, esfa::stream::SeekOffsetType seekType)
{
	stream->Seek(offset, seekType);
}

uint64_t Reader::GetBaseAddress()
{
	return stream->GetBaseAddress();
}

void Reader::Read(int32_t length)
{
	stream->Read(length);
}

void Reader::Read(char* buffer, int32_t length)
{
	stream->Read(buffer, length);
}

char Reader::ReadChar()
{
	return (char)stream->ReadByte();
}

int8_t Reader::ReadByte()
{
	return stream->ReadByte();
}

uint8_t Reader::ReadUByte()
{
	return (uint8_t)stream->ReadByte();
}

int16_t Reader::ReadInt16()
{
	int16_t result = 0;

	stream->Read((char*)&result, sizeof(int16_t));

	if (endianness != esfa::bit::Endianness::Native)
		result = BSWAP16(result);

	return result;
}

int32_t Reader::ReadInt32()
{
	int32_t result = 0;

	stream->Read((char*)&result, sizeof(int32_t));

	if (endianness != esfa::bit::Endianness::Native)
		result = BSWAP32(result);

	return result;
}

uint16_t Reader::ReadUInt16()
{
	uint16_t result = 0;

	stream->Read((char*)&result, sizeof(uint16_t));

	if (endianness != esfa::bit::Endianness::Native)
		result = BSWAP16(result);

	return result;
}

uint32_t Reader::ReadUInt32()
{
	uint32_t result = 0;

	stream->Read((char*)&result, sizeof(uint32_t));

	if (endianness != esfa::bit::Endianness::Native)
		result = BSWAP32(result);

	return result;
}

uint64_t Reader::ReadUInt64()
{
	uint64_t result = 0;

	stream->Read((char*)&result, sizeof(uint64_t));

	if (endianness != esfa::bit::Endianness::Native)
		result = BSWAP64(result);

	return result;
}

float Reader::ReadSingle()
{
	float result = NAN;

	stream->Read((char*)&result, sizeof(float));

	if (endianness != esfa::bit::Endianness::Native)
	{
		float tmp;
		char* dst = (char*)&tmp;
		char* src = (char*)&result;
		dst[3] = src[0]; dst[2] = src[1]; dst[1] = src[2]; dst[0] = src[3];
		result = tmp;
	}

	if (std::isnan(result))
		throw std::runtime_error("Reader::ReadSingle(): Error reading stream");

	return result;
}

double Reader::ReadDouble()
{
	double result = NAN;

	stream->Read((char*)&result, sizeof(double));

	if (endianness != esfa::bit::Endianness::Native)
	{
		double tmp;
		char* dst = (char*)&tmp;
		char* src = (char*)&result;
		dst[7] = src[0]; dst[6] = src[1]; dst[5] = src[2]; dst[4] = src[3];
		dst[3] = src[4]; dst[2] = src[5]; dst[1] = src[6]; dst[0] = src[7];
		result = tmp;
	}

	if (std::isnan(result))
		throw std::runtime_error("Reader::ReadDouble(): Error reading stream");

	return result;
}

std::string Reader::ReadString()
{
	std::string res;
	int numChars = ReadInt32();

	for (int i = 0; i < numChars; i++)
		res += ReadChar();

	return res;
}

}
