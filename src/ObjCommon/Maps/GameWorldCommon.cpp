#include "GameWorldCommon.h"

#include <format>

namespace game_world
{
    std::string GetFileNameForAssetName(const std::string& assetName)
    {
        std::string name = assetName;
        std::string suffix = ".d3dbsp";

        if (name.ends_with(suffix))
            name.erase(name.size() - suffix.size());

        return std::format("{}.paths", name);
    }
} // namespace game_world
