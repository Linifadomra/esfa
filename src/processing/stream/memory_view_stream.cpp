#include "esfa/processing/stream/memory_view_stream.hpp"

#include <cstring>
#include <stdexcept>
#include <string>

namespace esfa::stream {

MemoryViewStream::MemoryViewStream(const void* data, size_t length)
    : mData(static_cast<const uint8_t*>(data)), mSize(length), mPosition(0)
{
    SetBaseAddress(0);
}

MemoryViewStream::MemoryViewStream(std::span<const uint8_t> span)
    : mData(span.data()), mSize(span.size()), mPosition(0)
{
    SetBaseAddress(0);
}

uint64_t MemoryViewStream::GetLength()
{
    return mSize;
}

void MemoryViewStream::Seek(int64_t offset, SeekOffsetType seekType)
{
    int64_t base = 0;
    if (seekType == SeekOffsetType::Start)
    {
        base = 0;
    }
    else if (seekType == SeekOffsetType::Current)
    {
        base = static_cast<int64_t>(mPosition);
    }
    else if (seekType == SeekOffsetType::End)
    {
        base = static_cast<int64_t>(mSize);
    }
    else
    {
        throw std::invalid_argument("MemoryViewStream::Seek: unknown SeekOffsetType");
    }

    int64_t newPos = base + offset;
    if (newPos < 0)
    {
        throw std::out_of_range("MemoryViewStream::Seek: negative position");
    }
    mPosition = static_cast<size_t>(newPos);
}

std::unique_ptr<char[]> MemoryViewStream::Read(size_t length)
{
    auto buffer = std::make_unique<char[]>(length);
    Read(buffer.get(), length);
    return buffer;
}

void MemoryViewStream::Read(char* dest, size_t length)
{
    if (length == 0)
    {
        return;
    }
    if (mPosition + length > mSize)
    {
        throw std::runtime_error(
            "MemoryViewStream::Read: past end of buffer pos=" +
            std::to_string(mPosition) + " len=" + std::to_string(length) +
            " size=" + std::to_string(mSize));
    }
    std::memcpy(dest, mData + mPosition, length);
    mPosition += length;
}

int8_t MemoryViewStream::ReadByte()
{
    if (mPosition >= mSize)
    {
        throw std::runtime_error("MemoryViewStream::ReadByte: read past end of buffer");
    }
    return static_cast<int8_t>(mData[mPosition++]);
}

void MemoryViewStream::Write(char* /*destBuffer*/, size_t /*length*/)
{
    throw std::runtime_error("MemoryViewStream::Write: stream is read-only");
}

void MemoryViewStream::WriteByte(int8_t /*value*/)
{
    throw std::runtime_error("MemoryViewStream::WriteByte: stream is read-only");
}

void MemoryViewStream::Flush()
{
}

void MemoryViewStream::Close()
{
}

std::span<const uint8_t> MemoryViewStream::GetSpan() const
{
    return { mData, mSize };
}

std::span<const uint8_t> MemoryViewStream::GetSpan(size_t offset, size_t count) const
{
    if (offset + count > mSize)
    {
        throw std::out_of_range("MemoryViewStream::GetSpan: out of range");
    }
    return { mData + offset, count };
}

} // namespace esfa::stream
