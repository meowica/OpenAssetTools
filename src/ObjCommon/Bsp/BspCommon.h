#pragma once

#include "Game/T6/T6.h"

#include <string>

namespace bsp_common
{
    namespace clip_map
    {
        std::string GetFileNameForStaticModels(const std::string& assetName);
    }

    namespace com_world
    {
        std::string GetFileNameForPrimaryLights(const std::string& assetName);
    }

    namespace game_world
    {
        const char* GetNodeType(T6::nodeType type);

        std::string GetFileNameForAssetName(const std::string& assetName);
    }

    namespace map_ents
    {
        std::string GetEntsAssetName(const std::string& assetName);
        std::string GetTrigsAssetName(const std::string& assetName);
    }

    namespace gfx_world
    {
        std::string GetFileNameForReflectionProbes(const std::string& assetName);
    }

    namespace addon_map_ents
    {
        std::string GetFileNameForAssetName(const std::string& assetName);
    }
}
