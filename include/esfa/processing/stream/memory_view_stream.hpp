#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <span>
#include <string>

#include "esfa/processing/stream/stream.hpp"

namespace esfa::stream {

/**
 * @brief Stream wrapper over a non-owning read-only memory span or buffer.
 *
 * Implements esfa::stream::Stream with zero copying.
 */
class MemoryViewStream : public Stream {
public:
    MemoryViewStream() = default;

    MemoryViewStream(const void* data, size_t length);
    explicit MemoryViewStream(std::span<const uint8_t> span);

    uint64_t GetLength() override;

    void Seek(int64_t offset, SeekOffsetType seekType) override;

    std::unique_ptr<char[]> Read(size_t length) override;
    void Read(char* dest, size_t length) override;
    int8_t ReadByte() override;

    void Write(char* destBuffer, size_t length) override;
    void WriteByte(int8_t value) override;

    void Flush() override;
    void Close() override;

    std::span<const uint8_t> GetSpan() const;
    std::span<const uint8_t> GetSpan(size_t offset, size_t count) const;

    const uint8_t* Data() const { return mData; }
    size_t Size() const { return mSize; }
    size_t GetPosition() const { return mPosition; }

private:
    const uint8_t* mData = nullptr;
    size_t mSize = 0;
    size_t mPosition = 0;
};

} // namespace esfa::stream
