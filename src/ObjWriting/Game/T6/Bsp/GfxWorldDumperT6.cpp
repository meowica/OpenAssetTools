#include "GfxWorldDumperT6.h"

#include "Dumping/MapFile/MapFileDumper.h"
#include "Bsp/BSPCommon.h"

#include <format>

using namespace T6;

namespace
{
    void DumpReflectionProbes(AssetDumpingContext& context, const XAssetInfo<AssetGfxWorld::Type>& asset)
    {
        const auto* world = asset.Asset();
        const auto assetFile = context.OpenAssetFile(bsp_common::gfx_world::GetFileNameForReflectionProbes(asset.m_name));

        if (!assetFile)
            return;

        MapFileDumper mapFileDumper(*assetFile);
        mapFileDumper.Init();

        auto& stream = *assetFile;

        const auto& draw = world->draw;

        for (unsigned int i = 0; i < draw.reflectionProbeCount; ++i)
        {
            const auto& probe = draw.reflectionProbes[i];
            const auto& origin = probe.origin;

            mapFileDumper.BeginEntity();

            mapFileDumper.WriteKeyValue("origin", std::format("{:.1f} {:.1f} {:.1f}", origin.x, origin.y, origin.z));
            mapFileDumper.WriteKeyValue("classname", "reflection_probe");

            mapFileDumper.EndEntity();
        }
    }
} // namespace

namespace gfx_world
{
    void DumperT6::DumpAsset(AssetDumpingContext& context, const XAssetInfo<AssetGfxWorld::Type>& asset)
    {
        DumpReflectionProbes(context, asset);
    }
} // namespace gfx_world
