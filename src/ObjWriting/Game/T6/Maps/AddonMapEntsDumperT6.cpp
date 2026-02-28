#include "AddonMapEntsDumperT6.h"

#include "Dumping/MapFile/MapFileDumper.h"
#include "Maps/MapEntsCommon.h"

#include <format>

using namespace T6;

namespace addon_map_ents
{
    void DumperT6::DumpAsset(AssetDumpingContext& context, const XAssetInfo<AssetAddonMapEnts::Type>& asset)
    {
        const auto* addonMapEnts = asset.Asset();
        const auto assetFile = context.OpenAssetFile(map_ents::GetFileNameForAssetName(asset.m_name));

        if (!assetFile)
            return;

        MapFileDumper mapFileDumper(*assetFile);
        mapFileDumper.Init();

        auto& stream = *assetFile;
        stream.write(addonMapEnts->entityString, addonMapEnts->numEntityChars - 1);
    }
} // namespace addon_map_ents
