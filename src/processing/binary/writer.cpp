#include "esfa/processing/binary/writer.hpp"

namespace esfa::binary {

Writer::Writer(esfa::stream::Stream* nStream)
{
	stream.reset(nStream);
}

Writer::Writer(std::shared_ptr<esfa::stream::Stream> nStream)
{
	stream = nStream;
}

void Writer::SetEndianness(esfa::bit::Endianness endianness)
{
	this->endianness = endianness;
}

void Writer::Close()
{
	stream->Close();
}

std::shared_ptr<esfa::stream::Stream> Writer::GetStream()
{
	return stream;
}

uint64_t Writer::GetBaseAddress()
{
	return stream->GetBaseAddress();
}

uint64_t Writer::GetLength()
{
	return stream->GetLength();
}

void Writer::Seek(int64_t offset, esfa::stream::SeekOffsetType seekType)
{
	stream->Seek(offset, seekType);
}

void Writer::Write(int8_t value)
{
	stream->Write((char*)&value, sizeof(int8_t));
}

void Writer::Write(uint8_t value)
{
	stream->Write((char*)&value, sizeof(uint8_t));
}

void Writer::Write(int16_t value)
{
	if (endianness != esfa::bit::Endianness::Native)
		value = BSWAP16(value);

	stream->Write((char*)&value, sizeof(int16_t));
}

void Writer::Write(uint16_t value)
{
	if (endianness != esfa::bit::Endianness::Native)
		value = BSWAP16(value);

	stream->Write((char*)&value, sizeof(uint16_t));
}

void Writer::Write(int32_t value)
{
	if (endianness != esfa::bit::Endianness::Native)
		value = BSWAP32(value);

	stream->Write((char*)&value, sizeof(int32_t));
}

void Writer::Write(int32_t valueA, int32_t valueB)
{
	Write(valueA);
	Write(valueB);
}

void Writer::Write(uint32_t value)
{
	if (endianness != esfa::bit::Endianness::Native)
		value = BSWAP32(value);

	stream->Write((char*)&value, sizeof(uint32_t));
}

void Writer::Write(int64_t value)
{
	if (endianness != esfa::bit::Endianness::Native)
		value = BSWAP64(value);

	stream->Write((char*)&value, sizeof(int64_t));
}

void Writer::Write(uint64_t value)
{
	if (endianness != esfa::bit::Endianness::Native)
		value = BSWAP64(value);

	stream->Write((char*)&value, sizeof(uint64_t));
}

void Writer::Write(float value)
{
	if (endianness != esfa::bit::Endianness::Native)
	{
		float tmp;
		char* dst = (char*)&tmp;
		char* src = (char*)&value;
		dst[3] = src[0]; dst[2] = src[1]; dst[1] = src[2]; dst[0] = src[3];
		value = tmp;
	}

	stream->Write((char*)&value, sizeof(float));
}

void Writer::Write(double value)
{
	if (endianness != esfa::bit::Endianness::Native)
	{
		double tmp;
		char* dst = (char*)&tmp;
		char* src = (char*)&value;
		dst[7] = src[0]; dst[6] = src[1]; dst[5] = src[2]; dst[4] = src[3];
		dst[3] = src[4]; dst[2] = src[5]; dst[1] = src[6]; dst[0] = src[7];
		value = tmp;
	}

	stream->Write((char*)&value, sizeof(double));
}

void Writer::Write(const std::string& str)
{
	int strLen = str.size();
	Write(strLen);

	for (char c : str)
		stream->WriteByte(c);
}

void Writer::Write(char* srcBuffer, size_t length)
{
	stream->Write(srcBuffer, length);
}

}
