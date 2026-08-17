#pragma once

#include <any>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

#include "esfa/interface/asset.hpp"
#include "esfa/processing/binary/writer.hpp"
#include "esfa/processing/binary/reader.hpp"

namespace esfa::interface {

using AssetContext = std::any;

class Registry {
public:
    using ParseFn = std::function<
        std::shared_ptr<ParsedAsset>(esfa::binary::Reader&, const AssetMeta&, AssetContext&)>;
    using ExportFn = std::function<
        void(esfa::binary::Writer&, std::shared_ptr<ParsedAsset>, AssetContext&)>;

    void RegisterReader(std::string typeKey, ParseFn fn);
    void RegisterWriter(std::string typeKey, ExportFn fn);

    void OverrideReader(std::string typeKey, ParseFn fn);
    void OverrideWriter(std::string typeKey, ExportFn fn);

    std::shared_ptr<ParsedAsset> Parse(
        const std::string& typeKey, esfa::binary::Reader& reader,
        const AssetMeta& meta, AssetContext& ctx) const;

    void Export(
        const std::string& typeKey, esfa::binary::Writer& writer,
        std::shared_ptr<ParsedAsset> asset, AssetContext& ctx) const;

    bool ContainsReader(const std::string& id) const {
        return mReaders.contains(id);
    }

    bool ContainsWriter(const std::string& id) const {
        return mWriters.contains(id);
    }

private:
    std::unordered_map<std::string, ParseFn> mReaders;
    std::unordered_map<std::string, ExportFn> mWriters;
};

}
