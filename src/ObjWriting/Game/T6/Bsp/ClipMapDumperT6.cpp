#include "ClipMapDumperT6.h"
#include "Dumping/MapFile/MapFileDumper.h"

#include <format>

using namespace T6;

namespace
{
    void DumpDynModels(MapFileDumper& mapFileDumper, const clipMap_t* cm)
    {
        for (auto i = 0u; i < cm->dynEntCount[0]; ++i)
        {
            const auto& def = cm->dynEntDefList[0][i];
            if (!def.xModel)
                continue;

            const auto& pose = def.pose;

            vec3_t angles;
            Common::ToEulerAnglesDeg(&pose.quat, &angles);

            mapFileDumper.BeginEntity();
            mapFileDumper.WriteKeyValue("model", def.xModel->name);
            mapFileDumper.WriteKeyValue("origin", std::format("{:.1f} {:.1f} {:.1f}", pose.origin.x, pose.origin.y, pose.origin.z));
            mapFileDumper.WriteKeyValue("angles", std::format("{:.1f} {:.1f} {:.1f}", angles.x, angles.y, angles.z));
            mapFileDumper.WriteKeyValue("classname", "dyn_model");
            mapFileDumper.EndEntity();
        }
    }
} // namespace

namespace clip_map
{
    void DumperT6::DumpAsset(AssetDumpingContext& context, const XAssetInfo<AssetClipMap::Type>& asset)
    {
        auto* mapDumper = context.GetZoneAssetDumperState<MapFileDumperState>()->GetOrCreateDumper(context, asset.m_name);
        if (!mapDumper)
            return;

        DumpDynModels(*mapDumper, asset.Asset());
    }
} // namespace clip_map
