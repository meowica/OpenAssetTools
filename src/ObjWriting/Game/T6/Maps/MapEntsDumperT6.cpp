#include "MapEntsDumperT6.h"

#include "Dumping/MapFile/MapFileDumper.h"
#include "Maps/MapEntsCommon.h"

#include <format>

using namespace T6;

namespace map_ents
{
    DumperT6::DumperT6(const AssetPool<AssetMapEnts::Type>& pool)
        : AbstractAssetDumper(pool)
    {
    }

    void DumperT6::DumpAsset(AssetDumpingContext& context, const XAssetInfo<AssetMapEnts::Type>& asset)
    {
        const auto* mapEnts = asset.Asset();
        const auto assetFile = context.OpenAssetFile(map_ents::GetFileNameForAssetName(asset.m_name));

        if (!assetFile)
            return;

        MapFileDumper mapFileDumper(*assetFile);
        mapFileDumper.Init();

        auto& stream = *assetFile;
        stream.write(mapEnts->entityString, mapEnts->numEntityChars - 1);
    }
} // namespace map_ents
