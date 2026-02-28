#include "FontBinaryDumperT6.h"

#include "Game/T6/CommonT6.h"

#include <format>

using namespace T6;

namespace
{
} // namespace

namespace font
{
    void BinaryDumperT6::DumpAsset(AssetDumpingContext& context, const XAssetInfo<AssetFont::Type>& asset)
    {
        const auto* lightDef = asset.Asset();
        const auto assetFile = context.OpenAssetFile(asset.m_name);

        if (!assetFile)
            return;

        auto& stream = *assetFile;
        //stream.write(lightDef->attenuation.image->name, std::strlen(lightDef->attenuation.image->name) + 1);
    }
} // namespace font
