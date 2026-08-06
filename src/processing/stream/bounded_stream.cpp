#include "esfa/processing/stream/bounded_stream.hpp"

#include <stdexcept>

namespace esfa::stream {

BoundedStream::BoundedStream(std::shared_ptr<Stream> inner, uint64_t start, uint64_t size)
    : mInner(std::move(inner)), mStart(start), mSize(size)
{
    if (!mInner)
    {
        throw std::invalid_argument("BoundedStream: inner stream is null");
    }
    SetBaseAddress(start);
}

void BoundedStream::EnsureOpen() const
{
    if (mClosed)
    {
        throw std::runtime_error("BoundedStream: operation on closed stream");
    }
}

void BoundedStream::SyncInnerPosition()
{
    mInner->Seek(static_cast<int64_t>(mStart + mPosition), SeekOffsetType::Start);
}

uint64_t BoundedStream::GetLength()
{
    EnsureOpen();
    return mSize;
}

void BoundedStream::Seek(int64_t offset, SeekOffsetType seekType)
{
    EnsureOpen();

    int64_t target = 0;
    switch (seekType)
    {
    case SeekOffsetType::Start:
        target = offset;
        break;
    case SeekOffsetType::Current:
        target = static_cast<int64_t>(mPosition) + offset;
        break;
    case SeekOffsetType::End:
        target = static_cast<int64_t>(mSize) + offset;
        break;
    default:
        throw std::invalid_argument("BoundedStream::Seek: unknown SeekOffsetType");
    }

    if (target < 0 || static_cast<uint64_t>(target) > mSize)
    {
        throw std::out_of_range("BoundedStream::Seek: offset out of bounds for asset window");
    }

    mPosition = static_cast<uint64_t>(target);
}

std::unique_ptr<char[]> BoundedStream::Read(size_t length)
{
    auto buffer = std::make_unique<char[]>(length);
    Read(buffer.get(), length);
    return buffer;
}

void BoundedStream::Read(char* dest, size_t length)
{
    EnsureOpen();

    if (mPosition + length > mSize)
    {
        throw std::out_of_range("BoundedStream::Read: read past end of asset window");
    }

    SyncInnerPosition();
    mInner->Read(dest, length);
    mPosition += length;
}

int8_t BoundedStream::ReadByte()
{
    char byte;
    Read(&byte, 1);
    return static_cast<int8_t>(byte);
}

void BoundedStream::Write(char* srcBuffer, size_t length)
{
    EnsureOpen();

    if (mPosition + length > mSize)
    {
        throw std::out_of_range("BoundedStream::Write: write past end of asset window");
    }

    SyncInnerPosition();
    mInner->Write(srcBuffer, length);
    mPosition += length;
}

void BoundedStream::WriteByte(int8_t value)
{
    Write(reinterpret_cast<char*>(&value), 1);
}

void BoundedStream::Flush()
{
    EnsureOpen();
    mInner->Flush();
}

void BoundedStream::Close()
{
    mClosed = true;
}

}
