#include "GameWorldSpDumperT6.h"

#include "Dumping/MapFile/MapFileDumper.h"
#include "Bsp/BSPCommon.h"

#include <format>

using namespace T6;

namespace game_world_sp
{
    void DumperT6::DumpAsset(AssetDumpingContext& context, const XAssetInfo<AssetGameWorldSp::Type>& asset)
    {
        const auto* gameWorld = asset.Asset();
        const auto assetFile = context.OpenAssetFile(bsp_common::game_world::GetFileNameForAssetName(asset.m_name));

        if (!assetFile)
            return;

        MapFileDumper mapFileDumper(*assetFile);
        mapFileDumper.Init();

        auto& stream = *assetFile;

        auto& path = gameWorld->path;

        for (unsigned int i = 0; i < path.nodeCount; ++i)
        {
            const auto& node = path.nodes[i];
            const auto& origin = node.constant.vOrigin;
            const auto* classname = bsp_common::game_world::GetNodeType(node.constant.type);

            mapFileDumper.BeginEntity();

            mapFileDumper.WriteKeyValue("origin", std::format("{:.1f} {:.1f} {:.1f}", origin.x, origin.y, origin.z));
            mapFileDumper.WriteKeyValue("classname", classname);

            mapFileDumper.EndEntity();
        }
    }
} // namespace game_world_sp
