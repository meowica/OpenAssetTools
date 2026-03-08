#include "GameWorldSpDumperT6.h"

#include "MapFileDumperState.h"
#include "Bsp/BSPCommon.h"

#include <format>

using namespace T6;

namespace
{
    void DumpPathNodes(MapFileDumper& mapFileDumper, const GameWorldSp* world)
    {
        const auto& worldPath = world->path;

        for (auto i = 0u; i < worldPath.nodeCount; ++i)
        {
            const auto& pNode = worldPath.nodes[i];
            const auto& pNodeConst = pNode.constant;
            const auto className = bsp_common::GetNodeType(pNodeConst.type);

            mapFileDumper.BeginEntity();
            mapFileDumper.WriteKeyValue("origin", std::format("{:.1f} {:.1f} {:.1f}", pNodeConst.vOrigin.x, pNodeConst.vOrigin.y, pNodeConst.vOrigin.z));
            mapFileDumper.WriteKeyValue("classname", className);
            mapFileDumper.EndEntity();
        }
    }
} // namespace

namespace game_world_sp
{
    void DumperT6::DumpAsset(AssetDumpingContext& context, const XAssetInfo<AssetGameWorldSp::Type>& asset)
    {
        auto* mapDumper = context.GetZoneAssetDumperState<MapFileDumperState>()->GetOrCreateDumper(context, asset.m_name);
        if (!mapDumper)
            return;

        DumpPathNodes(*mapDumper, asset.Asset());
    }
} // namespace game_world_sp
