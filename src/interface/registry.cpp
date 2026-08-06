#include "esfa/interface/registry.hpp"

#include <stdexcept>

namespace esfa::interface {

void Registry::RegisterReader(std::string typeKey, ParseFn fn) {
    auto [it, inserted] = mReaders.try_emplace(std::move(typeKey), std::move(fn));
    if (!inserted) {
        throw std::runtime_error("esfa::Registry: reader already registered for type '" + it->first + "'");
    }
}

void Registry::RegisterWriter(std::string typeKey, ExportFn fn) {
    if (mWriters.find(typeKey) != mWriters.end()) {
        throw std::runtime_error("esfa::Registry: writer already registered for type '" + typeKey + "'");
    }
    mWriters.emplace(std::move(typeKey), std::move(fn));
}

std::shared_ptr<ParsedAsset> Registry::Parse(
    const std::string& typeKey, esfa::binary::Reader& reader,
    const AssetMeta& meta, std::any& ctx) const
{
    auto it = mReaders.find(typeKey);
    if (it == mReaders.end()) {
        throw std::runtime_error("esfa::Registry: no reader registered for type '" + typeKey + "'");
    }
    return it->second(reader, meta, ctx);
}

void Registry::Export(
    const std::string& typeKey, esfa::binary::Writer& writer,
    std::shared_ptr<ParsedAsset> asset, std::any& ctx) const
{
    auto it = mWriters.find(typeKey);
    if (it == mWriters.end()) {
        throw std::runtime_error("esfa::Registry: no writer registered for type '" + typeKey + "'");
    }
    it->second(writer, std::move(asset), ctx);
}

}
