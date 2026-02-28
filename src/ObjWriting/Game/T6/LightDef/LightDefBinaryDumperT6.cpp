#include "LightDefBinaryDumperT6.h"

#include "LightDef/LightDefCommon.h"
#include "Game/T6/CommonT6.h"

#include <format>

using namespace T6;

namespace light_def
{
    BinaryDumperT6::BinaryDumperT6(const AssetPool<AssetLightDef::Type>& pool)
        : AbstractAssetDumper(pool)
    {
    }

    void BinaryDumperT6::DumpAsset(AssetDumpingContext& context, const XAssetInfo<AssetLightDef::Type>& asset)
    {
        const auto* lightDef = asset.Asset();
        const auto assetFile = context.OpenAssetFile(light_def::GetFileNameForAsset(asset.m_name));

        if (!assetFile)
            return;

        auto& stream = *assetFile;
        stream.write(lightDef->attenuation.image->name, std::strlen(lightDef->attenuation.image->name) + 1);
    }
} // namespace light_def
