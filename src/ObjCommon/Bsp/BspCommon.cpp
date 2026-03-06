#include "BspCommon.h"

#include <format>

using namespace T6;

namespace bsp_common
{
    namespace clip_map
    {
        std::string GetFileNameForStaticModels(const std::string& assetName)
        {
            std::string name = assetName;
            std::string suffix = ".d3dbsp";

            if (name.ends_with(suffix))
                name.erase(name.size() - suffix.size());

            return std::format("{}_smodels.map", name);
        }
    } // namespace map_ents

    namespace com_world
    {
        std::string GetFileNameForPrimaryLights(const std::string& assetName)
        {
            std::string name = assetName;
            std::string suffix = ".d3dbsp";

            if (name.ends_with(suffix))
                name.erase(name.size() - suffix.size());

            return std::format("{}_primary_lights.map", name);
        }
    } // namespace clip_map

    namespace game_world
    {
        const char* GetNodeType(nodeType type)
        {
            switch (type)
            {
            case NODE_PATHNODE:
                return "node_pathnode";

            case NODE_COVER_STAND:
                return "node_cover_stand";

            case NODE_COVER_CROUCH:
                return "node_cover_crouch";

            case NODE_COVER_CROUCH_WINDOW:
                return "node_cover_crouch_window";

            case NODE_COVER_PRONE:
                return "node_cover_prone";

            case NODE_COVER_RIGHT:
                return "node_cover_right";

            case NODE_COVER_LEFT:
                return "node_cover_left";

            case NODE_COVER_PILLAR:
                return "node_cover_pillar";

            case NODE_AMBUSH:
                return "node_ambush";

            case NODE_EXPOSED:
                return "node_exposed";

            case NODE_CONCEALMENT_STAND:
                return "node_concealment_stand";

            case NODE_CONCEALMENT_CROUCH:
                return "node_concealment_crouch";

            case NODE_CONCEALMENT_PRONE:
                return "node_concealment_prone";

            case NODE_REACQUIRE:
                return "node_reacquire";

            case NODE_BALCONY:
                return "node_balcony";

            case NODE_SCRIPTED:
                return "node_scripted";

            case NODE_NEGOTIATION_BEGIN:
                return "node_negotiation_begin";

            case NODE_NEGOTIATION_END:
                return "node_negotiation_end";

            case NODE_TURRET:
                return "node_turret";

            case NODE_GUARD:
                return "node_guard";

            default:
                return "node_bad";
            }
        }

        std::string GetFileNameForAssetName(const std::string& assetName)
        {
            std::string name = assetName;
            std::string suffix = ".d3dbsp";

            if (name.ends_with(suffix))
                name.erase(name.size() - suffix.size());

            return std::format("{}_paths.map", name);
        }
    } // game_world

    namespace map_ents
    {
        std::string GetEntsAssetName(const std::string& assetName)
        {
            std::string name = assetName;
            std::string suffix = ".d3dbsp";

            if (name.ends_with(suffix))
                name.erase(name.size() - suffix.size());

            return std::format("{}.mapents", name);
        }

        std::string GetTrigsAssetName(const std::string& assetName)
        {
            std::string name = assetName;
            std::string suffix = ".d3dbsp";

            if (name.ends_with(suffix))
                name.erase(name.size() - suffix.size());

            return std::format("{}.triggers", name);
        }
    } // namespace map_ents

    namespace gfx_world
    {
        std::string GetFileNameForReflectionProbes(const std::string& assetName)
        {
            std::string name = assetName;
            std::string suffix = ".d3dbsp";

            if (name.ends_with(suffix))
                name.erase(name.size() - suffix.size());

            return std::format("{}_reflection_probes.map", name);
        }
    } // namespace map_ents

    namespace addon_map_ents
    {
        std::string GetFileNameForAssetName(const std::string& assetName)
        {
            std::string name = assetName;
            std::string suffix = ".d3dbsp";

            if (name.ends_with(suffix))
                name.erase(name.size() - suffix.size());

            return std::format("{}", name);
        }
    } // namespace addon_map_ents
} // namespace bsp_common
