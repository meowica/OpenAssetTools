#include "ClipMapDumperT6.h"

#include "Dumping/MapFile/MapFileDumper.h"
#include "Bsp/BSPCommon.h"

#include <format>

using namespace T6;

namespace
{
    void DumpStaticModels(AssetDumpingContext& context, const XAssetInfo<AssetClipMapPvs::Type>& asset)
    {
        const auto* cm = asset.Asset();
        const auto assetFile = context.OpenAssetFile(bsp_common::clip_map::GetFileNameForStaticModels(asset.m_name));

        if (!assetFile)
            return;

        MapFileDumper mapFileDumper(*assetFile);
        mapFileDumper.Init();

        auto& stream = *assetFile;

        for (unsigned int i = 0; i < cm->numStaticModels; ++i)
        {
            const auto& smodel = cm->staticModelList[i];
            const auto& origin = smodel.origin;

            mapFileDumper.BeginEntity();

            mapFileDumper.WriteKeyValue("model", smodel.xmodel->name);
            mapFileDumper.WriteKeyValue("origin", std::format("{:.1f} {:.1f} {:.1f}", origin.x, origin.y, origin.z));
            mapFileDumper.WriteKeyValue("classname", "misc_model");

            mapFileDumper.EndEntity();
        }
    }
} // namespace

namespace clip_map
{
    void DumperT6::DumpAsset(AssetDumpingContext& context, const XAssetInfo<AssetClipMapPvs::Type>& asset)
    {
        DumpStaticModels(context, asset);
    }
} // namespace clip_map
