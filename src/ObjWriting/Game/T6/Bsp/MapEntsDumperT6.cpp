#include "MapEntsDumperT6.h"

#include "Dumping/MapFile/MapFileDumper.h"
#include "Bsp/BSPCommon.h"

#include <format>

using namespace T6;

namespace
{
    void DumpMapEnts(AssetDumpingContext& context, const XAssetInfo<AssetMapEnts::Type>& asset)
    {
        const auto* mapEnts = asset.Asset();
        const auto assetFile = context.OpenAssetFile(bsp_common::GetFileNameForMapEnts(asset.m_name));

        if (!assetFile)
            return;

        MapFileDumper mapFileDumper(*assetFile);
        mapFileDumper.Init();

        auto& stream = *assetFile;
        stream.write(mapEnts->entityString, mapEnts->numEntityChars - 1);
    }

    void DumpMapTrigs(AssetDumpingContext& context, const XAssetInfo<AssetMapEnts::Type>& asset)
    {
        const auto* mapEnts = asset.Asset();
        const auto assetFile = context.OpenAssetFile(bsp_common::GetFileNameForMapTrigs(asset.m_name));

        if (!assetFile)
            return;

        MapFileDumper mapFileDumper(*assetFile);
        mapFileDumper.Init();

        auto& stream = *assetFile;

        stream.write("i wish", 6);
    }
} // namespace

namespace map_ents
{
    void DumperT6::DumpAsset(AssetDumpingContext& context, const XAssetInfo<AssetMapEnts::Type>& asset)
    {
        DumpMapEnts(context, asset);
        DumpMapTrigs(context, asset);
    }
} // namespace map_ents
