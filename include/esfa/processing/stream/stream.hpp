#pragma once

#include <cstdint>
#include <memory>

namespace esfa::stream {

enum class SeekOffsetType
{
	Start,
	Current,
	End
};

class Stream
{
public:
	virtual ~Stream() = default;
	virtual uint64_t GetLength() = 0;
	uint64_t GetBaseAddress() { return baseAddress; }

	virtual void Seek(int64_t offset, SeekOffsetType seekType) = 0;

	virtual std::unique_ptr<char[]> Read(size_t length) = 0;
	virtual void Read(char* dest, size_t length) = 0;
	virtual int8_t ReadByte() = 0;

	virtual void Write(char* destBuffer, size_t length) = 0;
	virtual void WriteByte(int8_t value) = 0;

	virtual void Flush() = 0;
	virtual void Close() = 0;

protected:
    void SetBaseAddress(uint64_t addr) { baseAddress = addr; }
	uint64_t baseAddress = 0;
};

}
