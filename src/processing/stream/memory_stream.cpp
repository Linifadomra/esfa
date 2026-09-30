#include "esfa/processing/stream/memory_stream.hpp"
#include "esfa/interface/errors.hpp"

#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <string>

namespace esfa::stream {

namespace {

using esfa::StreamError;
using esfa::StreamErrorKind;
using esfa::StreamType;

[[noreturn]] void Fail(StreamErrorKind kind, std::string message)
{
    throw StreamError(StreamType::Memory, kind, std::move(message));
}

}  // anonymous namespace

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
        Fail(StreamErrorKind::Seek,
             "negative position " + std::to_string(newPos));
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
    if (mPosition > mBuffer.size() || length > mBuffer.size() - mPosition)
    {
        Fail(StreamErrorKind::EndOfStream,
             "short read (past end of buffer) pos=" +
             std::to_string(mPosition) + " len=" + std::to_string(length) +
             " size=" + std::to_string(mBuffer.size()));
    }
    std::memcpy(dest, mBuffer.data() + mPosition, length);
    mPosition += length;
}

int8_t MemoryStream::ReadByte()
{
    if (mPosition >= mBuffer.size())
    {
        Fail(StreamErrorKind::EndOfStream, "read past end of buffer");
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
