#include "esfa/esfa.hpp"

#include "esfa/processing/stream/file_stream.hpp"

namespace esfa {

std::shared_ptr<interface::ParsedAsset> Processor::ParseAsset(
    const std::string& typeKey,
    const interface::AssetMeta& meta,
    std::any& ctx,
    bit::Endianness sourceEndianness)
{
    auto fileStream = std::make_shared<stream::FileStream>(
        meta.sourceFile, stream::FileMode::Read);

    fileStream->Seek(static_cast<int64_t>(meta.offset), stream::SeekOffsetType::Start);

    binary::Reader reader(fileStream);
    reader.SetEndianness(sourceEndianness);
    return mRegistry.Parse(typeKey, reader, meta, ctx);
}

void Processor::ExportAsset(
    const std::string& typeKey,
    std::shared_ptr<interface::ParsedAsset> asset,
    const std::filesystem::path& destFile,
    std::any& ctx,
    bit::Endianness targetEndianness)
{
    // todo: .tmp file left on disk in case of failure, wasteful
    auto tempFile = destFile;
    tempFile += ".tmp";

    {
        auto fileStream = std::make_shared<stream::FileStream>(
            tempFile, stream::FileMode::Write);

        binary::Writer writer(fileStream);
        writer.SetEndianness(targetEndianness);
        mRegistry.Export(typeKey, writer, std::move(asset), ctx);
        writer.Close();
    }

    std::filesystem::rename(tempFile, destFile);
}

}
