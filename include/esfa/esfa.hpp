#pragma once

#include "esfa/interface/registry.hpp"
#include "esfa/interface/asset.hpp"
#include "esfa/processing/stream/bounded_stream.hpp"

namespace esfa {

class Processor {
public:
    interface::Registry& GetRegistry() { return mRegistry; }

    std::shared_ptr<interface::ParsedAsset> ParseAsset(
        const std::string& typeKey,
        const interface::AssetMeta& meta,
        std::any& ctx,
        esfa::bit::Endianness sourceEndianness);

    void ExportAsset(
        const std::string& typeKey,
        std::shared_ptr<interface::ParsedAsset> asset,
        const std::filesystem::path& destFile,
        std::any& ctx,
        bit::Endianness targetEndianness,
        uint64_t maxSize = std::numeric_limits<uint64_t>::max());

private:
    interface::Registry mRegistry;
};

}
