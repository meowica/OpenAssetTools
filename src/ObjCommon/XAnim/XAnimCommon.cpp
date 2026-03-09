#include "XAnimCommon.h"

#include <format>

namespace xanim
{
    std::string GetJsonFileNameForAssetName(const std::string& assetName)
    {
        return std::format("xanim/{}.json", assetName);
    }
} // namespace xanim
