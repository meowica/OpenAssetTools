#pragma once

#include "Game/T6/T6.h"

#include <string>

namespace bsp_common
{
    namespace clip_map
    {
        std::string GetFileNameForStaticModels(const std::string& assetName);
    } // namespace clip_map

    namespace com_world
    {
        std::string GetFileNameForPrimaryLights(const std::string& assetName);
    } // namespace com_world

    namespace game_world
    {
        const char* GetNodeType(T6::nodeType type);

        std::string GetFileNameForAssetName(const std::string& assetName);
    } // namespace game_world

    namespace map_ents
    {
        std::string GetFileNameForAssetName(const std::string& assetName);
    } // namespace map_ents

    namespace gfx_world
    {
        std::string GetFileNameForReflectionProbes(const std::string& assetName);
    } // namespace gfx_world
} // namespace bsp_common
