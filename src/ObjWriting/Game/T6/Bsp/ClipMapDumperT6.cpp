#include "ClipMapDumperT6.h"

#include "Dumping/MapFile/MapFileDumper.h"
#include "Bsp/BSPCommon.h"

#include <format>

using namespace T6;

namespace
{
    //void DumpDynModels(AssetDumpingContext& context, const XAssetInfo<AssetClipMapPvs::Type>& asset)
    //{
    //    const auto* cm = asset.Asset();
    //    const auto assetFile = context.OpenAssetFile(bsp_common::clip_map::GetFileNameForDynModels(asset.m_name));

    //    if (!assetFile)
    //        return;

    //    MapFileDumper mapFileDumper(*assetFile);
    //    mapFileDumper.Init();

    //    auto& stream = *assetFile;

    //    if (cm->dynEntDefList[0] && cm->dynEntCount[0])
    //    {
    //        for (auto i = 0; i < cm->dynEntCount[0]; i++)
    //        {
    //            if (auto next = &cm->dynEntDefList[0][i]; next)
    //            {
    //                mapFileDumper.BeginEntity();

    //                mapFileDumper.WriteKeyValue("model", "void" /*next->xModel->name*/);
    //                mapFileDumper.WriteKeyValue("origin", std::format("{:.1f} {:.1f} {:.1f}", next->pose.origin.x, next->pose.origin.y, next->pose.origin.z));
    //                mapFileDumper.WriteKeyValue("classname", "dyn_model");

    //                mapFileDumper.EndEntity();
    //            }
    //        }
    //    }
    //}
} // namespace

namespace clip_map
{
    void DumperT6::DumpAsset(AssetDumpingContext& context, const XAssetInfo<AssetClipMap::Type>& asset)
    {
        //DumpDynModels(context, asset);
    }
} // namespace clip_map
