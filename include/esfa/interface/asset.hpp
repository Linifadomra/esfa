
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
    uint32_t offset = 0;
    uint32_t size = 0;
    std::filesystem::path sourceFile;

    // Allow asset to grow during swap (e.g., uncompressed formats)
    // If not set, defaults to input size
    std::optional<uint64_t> maxOutputSize = std::nullopt;

    AssetMeta() = default;
    AssetMeta(std::string n, uint32_t off, uint32_t sz, std::filesystem::path src,
              std::optional<uint64_t> maxOut = std::nullopt)
        : name(std::move(n)), offset(off), size(sz), sourceFile(std::move(src)), maxOutputSize(maxOut) {}
};

}
