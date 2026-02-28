#include "FontCommon.h"

#include <format>

namespace font
{
    std::string GetJsonFileNameForAsset(const std::string& assetName)
    {
        return std::format("{}.json", assetName);
    }
} // namespace font
