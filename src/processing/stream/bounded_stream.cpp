#include "esfa/processing/stream/bounded_stream.hpp"
#include "esfa/interface/errors.hpp"

#include <stdexcept>
#include <string>

namespace esfa::stream {

namespace {

using esfa::StreamError;
using esfa::StreamErrorKind;
using esfa::StreamType;

[[noreturn]] void Fail(StreamErrorKind kind, std::string message)
{
    throw StreamError(StreamType::Bounded, kind, std::move(message));
}

}  // anonymous namespace

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
        Fail(StreamErrorKind::Closed, "operation on closed stream");
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
    if (seekType == SeekOffsetType::Start)
    {
        target = offset;
    }
    else if (seekType == SeekOffsetType::Current)
    {
        target = static_cast<int64_t>(mPosition) + offset;
    }
    else if (seekType == SeekOffsetType::End)
    {
        target = static_cast<int64_t>(mSize) + offset;
    }
    else
    {
        throw std::invalid_argument("BoundedStream::Seek: unknown SeekOffsetType");
    }

    if (target < 0 || static_cast<uint64_t>(target) > mSize)
    {
        Fail(StreamErrorKind::Seek,
             "offset " + std::to_string(target) + " out of bounds for window of size " +
             std::to_string(mSize));
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
        Fail(StreamErrorKind::EndOfStream,
             "read of " + std::to_string(length) + " bytes at position " +
             std::to_string(mPosition) + " passes end of window (size " +
             std::to_string(mSize) + ")");
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
        Fail(StreamErrorKind::EndOfStream,
             "write of " + std::to_string(length) + " bytes at position " +
             std::to_string(mPosition) + " passes end of window (size " +
             std::to_string(mSize) + ")");
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
    if (!mClosed)
    {
        mInner->Flush();
    }
    mClosed = true;
}

}
