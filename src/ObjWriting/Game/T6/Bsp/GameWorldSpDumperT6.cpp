#include "GameWorldSpDumperT6.h"

#include "Bsp/BSPCommon.h"

#include <format>

using namespace T6;

namespace game_world_sp
{
    void DumperT6::DumpAsset(AssetDumpingContext& context, const XAssetInfo<AssetGameWorldSp::Type>& asset)
    {
        const auto* gameWorldSp = asset.Asset();
        const auto assetFile = context.OpenAssetFile(bsp_common::game_world::GetFileNameForAssetName(asset.m_name));

        if (!assetFile)
            return;

        auto& stream = *assetFile;

        auto& path = gameWorldSp->path;

        for (unsigned int i = 0; i < path.nodeCount; ++i)
        {
            const auto& node = path.nodes[i];
            const auto& origin = node.constant.vOrigin;
            const auto* classname = bsp_common::game_world::GetNodeType(node.constant.type);

            stream << "{\n";
            stream << "\"origin\" \"" << origin.x << " " << origin.y << " " << origin.z << "\"\n";
            stream << "\"classname\" \"" << classname << "\"\n";
            stream << "}\n";
        }
    }
} // namespace game_world_sp
