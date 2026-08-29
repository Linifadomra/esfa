#pragma once

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <stdexcept>

#include "esfa/processing/stream/stream.hpp"

namespace esfa::stream {

enum class FileMode { Read, Write, ReadWrite };

class FileStream : public Stream {
public:
    FileStream(const std::filesystem::path& path, FileMode mode);
    ~FileStream() override;

    uint64_t GetLength() override;

    void Seek(int64_t offset, SeekOffsetType seekType) override;

    std::unique_ptr<char[]> Read(size_t length) override;
    void Read(char* dest, size_t length) override;
    int8_t ReadByte() override;

    void Write(char* srcBuffer, size_t length) override;
    void WriteByte(int8_t value) override;

    void Flush() override;
    void Close() override;

private:
    std::filesystem::path mPath;
    std::fstream mFile;
    bool mClosed = false;

    void EnsureOpen() const;
};

}
