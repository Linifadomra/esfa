#include "esfa/processing/stream/file_stream.hpp"
#include "esfa/interface/errors.hpp"

namespace esfa::stream {

namespace {

using esfa::StreamError;
using esfa::StreamErrorKind;
using esfa::StreamType;

[[noreturn]] void Fail(StreamErrorKind kind, std::string message)
{
    throw StreamError(StreamType::File, kind, std::move(message));
}

std::ios::openmode ToOpenMode(FileMode mode)
{
    switch (mode)
    {
    case FileMode::Read:
        return std::ios::in | std::ios::binary;
    case FileMode::Write:
        return std::ios::out | std::ios::binary | std::ios::trunc;
    case FileMode::ReadWrite:
        return std::ios::in | std::ios::out | std::ios::binary;
    }
    throw std::invalid_argument("FileStream: unknown FileMode");
}

}  // anonymous namespace

FileStream::FileStream(const std::filesystem::path& path, FileMode mode)
    : mPath(path)
{
    if (mode == FileMode::ReadWrite && !std::filesystem::exists(mPath))
    {
        std::ofstream create(mPath, std::ios::binary);
        if (!create.is_open())
        {
            Fail(StreamErrorKind::Open, "failed to create '" + mPath.string() + "'");
        }
        create.close();
    }

    mFile.open(mPath, ToOpenMode(mode));
    if (!mFile.is_open())
    {
        Fail(StreamErrorKind::Open, "failed to open '" + mPath.string() + "'");
    }
    SetBaseAddress(0);
}

FileStream::~FileStream()
{
    try { Close(); } catch (...) {}
}

void FileStream::EnsureOpen() const
{
    if (mClosed || !mFile.is_open())
    {
        Fail(StreamErrorKind::Closed, "operation on closed stream '" + mPath.string() + "'");
    }
}

uint64_t FileStream::GetLength()
{
    EnsureOpen();

    auto current = mFile.tellg();
    mFile.seekg(0, std::ios::end);
    auto end = mFile.tellg();
    mFile.seekg(current);

    if (end < 0)
    {
        Fail(StreamErrorKind::Seek, "GetLength failed on '" + mPath.string() + "'");
    }

    return static_cast<uint64_t>(end);
}

void FileStream::Seek(int64_t offset, SeekOffsetType seekType)
{
    EnsureOpen();

    std::ios::seekdir dir;
    switch (seekType)
    {
    case SeekOffsetType::Start:   dir = std::ios::beg; break;
    case SeekOffsetType::Current: dir = std::ios::cur; break;
    case SeekOffsetType::End:     dir = std::ios::end; break;
    default:
        throw std::invalid_argument("FileStream::Seek: unknown SeekOffsetType");
    }

    mFile.seekg(offset, dir);
    mFile.seekp(offset, dir);

    if (mFile.fail())
    {
        Fail(StreamErrorKind::Seek, "seek failed on '" + mPath.string() + "'");
    }
}

std::unique_ptr<char[]> FileStream::Read(size_t length)
{
    auto buffer = std::make_unique<char[]>(length);
    Read(buffer.get(), length);
    return buffer;
}

void FileStream::Read(char* dest, size_t length)
{
    EnsureOpen();

    mFile.read(dest, static_cast<std::streamsize>(length));
    if (static_cast<size_t>(mFile.gcount()) != length)
    {
        Fail(StreamErrorKind::EndOfStream,
             "short read on '" + mPath.string() + "': requested " +
             std::to_string(length) + " bytes, got " +
             std::to_string(mFile.gcount()));
    }
}

int8_t FileStream::ReadByte()
{
    EnsureOpen();

    char byte;
    mFile.read(&byte, 1);
    if (mFile.gcount() != 1)
    {
        Fail(StreamErrorKind::EndOfStream, "read past end of '" + mPath.string() + "'");
    }

    return static_cast<int8_t>(byte);
}

void FileStream::Write(char* srcBuffer, size_t length)
{
    EnsureOpen();

    mFile.write(srcBuffer, static_cast<std::streamsize>(length));
    if (mFile.fail())
    {
        Fail(StreamErrorKind::Write, "short write on '" + mPath.string() + "'");
    }
}

void FileStream::WriteByte(int8_t value)
{
    Write(reinterpret_cast<char*>(&value), 1);
}

void FileStream::Flush()
{
    EnsureOpen();
    mFile.flush();
}

void FileStream::Close()
{
    if (!mClosed && mFile.is_open())
    {
        mFile.close();
    }
    mClosed = true;
}

}  // namespace esfa::stream
