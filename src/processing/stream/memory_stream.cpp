#include "esfa/processing/stream/memory_stream.hpp"

#include <algorithm>
#include <cstring>
#include <stdexcept>

namespace esfa::stream {

MemoryStream::MemoryStream(const std::vector<uint8_t>& data)
    : mBuffer(reinterpret_cast<const char*>(data.data()),
              reinterpret_cast<const char*>(data.data()) + data.size())
{
    SetBaseAddress(0);
}

MemoryStream::MemoryStream(std::vector<uint8_t>&& data)
    : mBuffer(reinterpret_cast<const char*>(data.data()),
              reinterpret_cast<const char*>(data.data()) + data.size())
{
    SetBaseAddress(0);
}

uint64_t MemoryStream::GetLength()
{
    return mBuffer.size();
}

void MemoryStream::Seek(int64_t offset, SeekOffsetType seekType)
{
    int64_t base = 0;
    switch (seekType)
    {
    case SeekOffsetType::Start:
        base = 0;
        break;
    case SeekOffsetType::Current:
        base = static_cast<int64_t>(mPosition);
        break;
    case SeekOffsetType::End:
        base = static_cast<int64_t>(mBuffer.size());
        break;
    default:
        throw std::invalid_argument("MemoryStream::Seek: unknown SeekOffsetType");
    }

    int64_t newPos = base + offset;
    if (newPos < 0)
    {
        throw std::out_of_range("MemoryStream::Seek: negative position");
    }

    mPosition = static_cast<size_t>(newPos);
}

std::unique_ptr<char[]> MemoryStream::Read(size_t length)
{
    auto buffer = std::make_unique<char[]>(length);
    Read(buffer.get(), length);
    return buffer;
}

void MemoryStream::Read(char* dest, size_t length)
{
    if (length == 0)
    {
        return;
    }
    if (mPosition + length > mBuffer.size())
    {
        std::string msg =
            "MemoryStream::Read: position " + std::to_string(mPosition) +
            " + length " + std::to_string(length) +
            " = " + std::to_string(mPosition + length) +
            " > buffer size " + std::to_string(mBuffer.size());

        throw std::runtime_error(msg);
    }
    std::memcpy(dest, mBuffer.data() + mPosition, length);
    mPosition += length;
}

int8_t MemoryStream::ReadByte()
{
    if (mPosition >= mBuffer.size())
    {
        throw std::runtime_error("MemoryStream::ReadByte: read past end of buffer");
    }
    return static_cast<int8_t>(mBuffer[mPosition++]);
}

void MemoryStream::Write(char* srcBuffer, size_t length)
{
    if (length == 0)
    {
        return;
    }

    size_t end = mPosition + length;
    if (end > mBuffer.size())
    {
        mBuffer.resize(end, 0); // zero-fill any gap between old end and mPosition
    }
    std::memcpy(mBuffer.data() + mPosition, srcBuffer, length);
    mPosition += length;
}

void MemoryStream::WriteByte(int8_t value)
{
    Write(reinterpret_cast<char*>(&value), 1);
}

void MemoryStream::Flush()
{
    // No-op: nothing to flush for an in-memory buffer.
}

void MemoryStream::Close()
{
    // No-op: no OS resource to release. Buffer contents remain valid and
    // readable via GetBuffer() after Close(), unlike FileStream.
}

std::vector<uint8_t> MemoryStream::GetBuffer() const
{
    return std::vector<uint8_t>(mBuffer.begin(), mBuffer.end());
}

} // namespace esfa::stream
