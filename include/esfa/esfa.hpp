// include/esfa/esfa.hpp
#pragma once

#include "esfa/interface/registry.hpp"
#include "esfa/interface/asset.hpp"

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
        bit::Endianness targetEndianness);

private:
    interface::Registry mRegistry;
};

}
