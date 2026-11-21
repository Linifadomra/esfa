#include "esfa/esfa.hpp"

#include "esfa/processing/stream/file_stream.hpp"
#include "esfa/processing/stream/memory_stream.hpp"

#include <fstream>
#include <stdexcept>

namespace esfa {

std::shared_ptr<interface::ParsedAsset> Processor::ParseAsset(
    const std::string& typeKey,
    const interface::AssetMeta& meta,
    std::any& ctx,
    bit::Endianness sourceEndianness)
{
    auto fileStream = std::make_shared<stream::FileStream>(
        meta.sourceFile, stream::FileMode::Read);

    auto bounded = std::make_shared<stream::BoundedStream>(
        fileStream, meta.offset, meta.size);

    binary::Reader reader(bounded);
    reader.SetEndianness(sourceEndianness);
    return mRegistry.Parse(typeKey, reader, meta, ctx);
}

std::shared_ptr<interface::ParsedAsset> Processor::ParseAsset(
    const std::string& typeKey,
    std::shared_ptr<stream::Stream> inStream,
    const interface::AssetMeta& meta,
    std::any& ctx,
    bit::Endianness sourceEndianness)
{
    auto bounded = std::make_shared<stream::BoundedStream>(
        inStream, meta.offset, meta.size);

    binary::Reader reader(bounded);
    reader.SetEndianness(sourceEndianness);
    return mRegistry.Parse(typeKey, reader, meta, ctx);
}

void Processor::ExportAsset(
    const std::string& typeKey,
    std::shared_ptr<interface::ParsedAsset> asset,
    const std::filesystem::path& destFile,
    std::any& ctx,
    bit::Endianness targetEndianness,
    uint64_t maxSize)
{
    const auto parent = destFile.parent_path();

    if (!parent.empty())
    {
        std::filesystem::create_directories(parent);
    }

    auto tempFile = destFile;
    tempFile += ".tmp";

    try {
        {
            auto memoryStream = std::make_shared<stream::MemoryStream>();

            auto bounded = std::make_shared<stream::BoundedStream>(
                memoryStream, 0, maxSize);

            binary::Writer writer(bounded);
            writer.SetEndianness(targetEndianness);
            mRegistry.Export(typeKey, writer, std::move(asset), ctx);
            writer.Close();

            const std::vector<uint8_t> bytes = memoryStream->GetBuffer();
            std::ofstream out(tempFile, std::ios::binary | std::ios::trunc);
            out.write(reinterpret_cast<const char*>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
            out.close();
            if (!out) {
                throw std::runtime_error("failed to write '" + tempFile.string() + "'");
            }
        }
        std::error_code renameError;
        std::filesystem::rename(tempFile, destFile, renameError);
        if (renameError) {
            std::filesystem::remove(destFile);
            std::filesystem::rename(tempFile, destFile);
        }
    } catch(...) {
        std::error_code ec;
        std::filesystem::remove(tempFile, ec);
        throw;
    }
}

void Processor::ExportAsset(
    const std::string& typeKey,
    std::shared_ptr<interface::ParsedAsset> asset,
    std::shared_ptr<stream::Stream> outStream,
    std::any& ctx,
    bit::Endianness targetEndianness,
    uint64_t maxSize)
{
    auto bounded = std::make_shared<stream::BoundedStream>(outStream, 0, maxSize);
    binary::Writer writer(bounded);
    writer.SetEndianness(targetEndianness);
    mRegistry.Export(typeKey, writer, std::move(asset), ctx);
    writer.Close();
}

}
