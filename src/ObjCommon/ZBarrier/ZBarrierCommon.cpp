#include "ZBarrierCommon.h"

#include <format>

namespace zbarrier
{
    std::string GetFileNameForAssetName(const std::string& assetName)
    {
        return std::format("zbarrier/{}", assetName);
    }
} // namespace zbarrier
