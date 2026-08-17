#include <cassert>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>

#include "esfa/processing/binary/reader.hpp"
#include "esfa/processing/stream/file_stream.hpp"

namespace {

void WriteBigEndianTestFile(const std::filesystem::path& path)
{
    std::ofstream out(path, std::ios::binary | std::ios::trunc);
    if (!out.is_open())
    {
        throw std::runtime_error("test setup: failed to create '" + path.string() + "'");
    }

    // uint8_t  0x00  -> 0xAB
    uint8_t byteVal = 0xAB;
    out.put(static_cast<char>(byteVal));

    // uint16_t 0x01  -> 0x1234
    out.put(0x12);
    out.put(0x34);

    // int32_t  0x03  -> -1  (0xFFFFFFFF)
    out.put(0xFF);
    out.put(0xFF);
    out.put(0xFF);
    out.put(0xFF);

    // uint32_t 0x07  -> 0xDEADBEEF
    out.put(0xDE);
    out.put(0xAD);
    out.put(0xBE);
    out.put(0xEF);

    // uint64_t 0x0B  -> 0x0102030405060708
    out.put(0x01);
    out.put(0x02);
    out.put(0x03);
    out.put(0x04);
    out.put(0x05);
    out.put(0x06);
    out.put(0x07);
    out.put(0x08);

    // float    0x13  -> 1.5f big-endian bytes: 3F C0 00 00
    out.put(0x3F);
    out.put(0xC0);
    out.put(0x00);
    out.put(0x00);

    out.close();
}

}  // namespace

int main()
{
    const std::filesystem::path testFile =
        std::filesystem::temp_directory_path() / "esfa_reader_test.bin";

    WriteBigEndianTestFile(testFile);

    {
        auto fileStream = std::make_shared<esfa::stream::FileStream>(
            testFile, esfa::stream::FileMode::Read);

        esfa::binary::Reader reader(fileStream);
        reader.SetEndianness(esfa::bit::Endianness::Big);

        uint8_t byteVal = reader.ReadUByte();
        assert(byteVal == 0xAB);

        uint16_t u16 = reader.ReadUInt16();
        assert(u16 == 0x1234);

        int32_t i32 = reader.ReadInt32();
        assert(i32 == -1);

        uint32_t u32 = reader.ReadUInt32();
        assert(u32 == 0xDEADBEEF);

        uint64_t u64 = reader.ReadUInt64();
        assert(u64 == 0x0102030405060708ULL);

        float f = reader.ReadSingle();
        assert(f == 1.5f);

        reader.Seek(0, esfa::stream::SeekOffsetType::Start);
        uint8_t rewound = reader.ReadUByte();
        assert(rewound == 0xAB);

        reader.Close();
    }

    std::filesystem::remove(testFile);

    std::cout << "reader_test: all assertions passed\n";
    return 0;
}
