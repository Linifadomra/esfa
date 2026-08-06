#pragma once

#include <cstdint>
#include <memory>

#include "esfa/processing/stream/stream.hpp"

namespace esfa::stream {

// Does NOT own or close the inner stream 
class BoundedStream : public Stream
{
public:
    BoundedStream(std::shared_ptr<Stream> inner, uint64_t start, uint64_t size);

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
    std::shared_ptr<Stream> mInner;
    uint64_t mStart;
    uint64_t mSize;
    uint64_t mPosition = 0; // relative to mStart, in [0, mSize]
    bool mClosed = false;

    void EnsureOpen() const;

    void SyncInnerPosition();
};

}
