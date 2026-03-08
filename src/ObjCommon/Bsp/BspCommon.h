#pragma once

#include "Game/T6/T6.h"

#include <string>

namespace bsp_common
{
    std::string GetNodeType(T6::nodeType type);

    std::string GetFileNameForMap(const std::string& assetName);
    std::string GetFileNameForMapEnts(const std::string& assetName);
    std::string GetFileNameForMapTrigs(const std::string& assetName);
    std::string GetFileNameForAddonMapEnts(const std::string& assetName);
}
