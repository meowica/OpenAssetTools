#include "AddonMapEntsDumperT6.h"

#include <format>

using namespace T6;

namespace addon_map_ents
{
    DumperT6::DumperT6(const AssetPool<AssetAddonMapEnts::Type>& pool)
        : AbstractAssetDumper(pool)
    {
    }

    void DumperT6::DumpAsset(AssetDumpingContext& context, const XAssetInfo<AssetAddonMapEnts::Type>& asset)
    {
        const auto* addonMapEnts = asset.Asset();

        const auto addonMapEntsFile = context.OpenAssetFile(std::format("{}", addonMapEnts->name));

        if (!addonMapEntsFile)
            return;

        auto& stream = *addonMapEntsFile;
        stream.write(addonMapEnts->entityString, addonMapEnts->numEntityChars - 1);
    }
} // namespace addon_map_ents
