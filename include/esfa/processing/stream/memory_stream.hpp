#pragma once

#include <cstdint>
#include <cstddef>
#include <vector>

#include "esfa/processing/stream/stream.hpp"

namespace esfa::stream {

class MemoryStream : public Stream {
public:
    MemoryStream() = default;

    explicit MemoryStream(const std::vector<uint8_t>& data);
    explicit MemoryStream(std::vector<uint8_t>&& data);

    uint64_t GetLength() override;

    void Seek(int64_t offset, SeekOffsetType seekType) override;

    std::unique_ptr<char[]> Read(size_t length) override;
    void Read(char* dest, size_t length) override;
    int8_t ReadByte() override;

    void Write(char* srcBuffer, size_t length) override;
    void WriteByte(int8_t value) override;

    void Flush() override;
    void Close() override;

    std::vector<uint8_t> GetBuffer() const;

private:
    std::vector<char> mBuffer;
    size_t mPosition = 0;
};

} // namespace esfa::stream
