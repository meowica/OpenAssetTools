#include "GameWorldMpDumperT6.h"

#include "Dumping/MapFile/MapFileDumper.h"
#include "Maps/GameWorldCommon.h"

#include <format>

using namespace T6;

namespace
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
} // namespace

namespace game_world_mp
{
    void DumperT6::DumpAsset(AssetDumpingContext& context, const XAssetInfo<AssetGameWorldMp::Type>& asset)
    {
        const auto* gameWorldMp = asset.Asset();
        const auto assetFile = context.OpenAssetFile(game_world::GetFileNameForAssetName(asset.m_name));

        if (!assetFile)
            return;

        auto& stream = *assetFile;

        auto& path = gameWorldMp->path;

        for (unsigned int i = 0; i < path.nodeCount; ++i)
        {
            const pathnode_t& node = path.nodes[i];

            const auto& origin = node.constant.vOrigin;
            const auto* classname = GetNodeType(node.constant.type);

            stream << "{\n";
            stream << "\"origin\" \"" << origin.x << " " << origin.y << " " << origin.z << "\"\n";
            stream << "\"classname\" \"" << classname << "\"\n";
            stream << "}\n";
        }
    }
} // namespace game_world_mp
