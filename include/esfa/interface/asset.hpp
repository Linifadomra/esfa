
#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>

namespace esfa::interface {

// We never know what's inside a parsed asset.
class ParsedAsset {
public:
    virtual ~ParsedAsset() = default;
};

// Minimal per-asset info the registry needs to pass through.
struct AssetMeta {
    std::string name;
    uint32_t offset;
    uint32_t size;
    std::filesystem::path sourceFile;
};

}
