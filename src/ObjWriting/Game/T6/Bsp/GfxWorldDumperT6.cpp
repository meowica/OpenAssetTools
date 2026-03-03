#include "GfxWorldDumperT6.h"

#include "Bsp/BSPCommon.h"

#include <format>

using namespace T6;

namespace gfx_world
{
    void DumperT6::DumpAsset(AssetDumpingContext& context, const XAssetInfo<AssetGfxWorld::Type>& asset)
    {
        const auto* mapEnts = asset.Asset();
        const auto assetFile = context.OpenAssetFile(bsp_common::gfx_world::GetFileNameForReflectionProbes(asset.m_name));

        if (!assetFile)
            return;

        auto& stream = *assetFile;

        const auto& draw = mapEnts->draw;

        for (unsigned int i = 0; i < draw.reflectionProbeCount; ++i)
        {
            const GfxReflectionProbe& probe = draw.reflectionProbes[i];

            const float x = probe.origin.x;
            const float y = probe.origin.y;
            const float z = probe.origin.z;

            stream << "{\n";
            stream << "\"origin\" \"" << x << " " << y << " " << z << "\"\n";
            stream << "\"classname\" \"reflection_probe\"\n";
            stream << "}\n";
        }
    }
} // namespace gfx_world
