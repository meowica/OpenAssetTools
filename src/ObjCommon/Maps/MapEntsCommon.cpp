#include "MapEntsCommon.h"

#include <format>

namespace map_ents
{
    std::string GetFileNameForAssetName(const std::string& assetName)
    {
        std::string name = assetName;

        constexpr std::string_view suffix = ".d3dbsp";

        if (name.ends_with(suffix))
            name.erase(name.size() - suffix.size());

        return std::format("{}.mapents", name);
    }
} // namespace map_ents
