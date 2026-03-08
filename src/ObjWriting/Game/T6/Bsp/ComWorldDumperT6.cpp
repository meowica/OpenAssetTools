#include "ComWorldDumperT6.h"

#include "Dumping/MapFile/MapFileDumper.h"

#include <format>

using namespace T6;

namespace
{
    void DumpPrimaryLights(MapFileDumper& mapFileDumper, const ComWorld* world)
    {
        for (auto i = 0u; i < world->primaryLightCount; ++i)
        {
            const auto& pLight = world->primaryLights[i];

            mapFileDumper.BeginEntity();
            mapFileDumper.WriteKeyValue("origin", std::format("{:.1f} {:.1f} {:.1f}", pLight.origin.x, pLight.origin.y, pLight.origin.z));
            mapFileDumper.WriteKeyValue("def", pLight.defName ? pLight.defName : "unknown");
            mapFileDumper.WriteKeyValue("radius", std::format("{:.1f}", pLight.radius));
            mapFileDumper.WriteKeyValue("_color", std::format("{:.4f} {:.4f} {:.4f}", pLight.color.x, pLight.color.y, pLight.color.z));
            mapFileDumper.WriteKeyValue("intensity", "1.0");
            mapFileDumper.WriteKeyValue("classname", "light");
            mapFileDumper.EndEntity();
        }
    }
} // namespace

namespace com_world
{
    void DumperT6::DumpAsset(AssetDumpingContext& context, const XAssetInfo<AssetComWorld::Type>& asset)
    {
        auto* mapDumper = context.GetZoneAssetDumperState<MapFileDumperState>()->GetOrCreateDumper(context, asset.m_name);
        if (!mapDumper)
            return;

        DumpPrimaryLights(*mapDumper, asset.Asset());
    }
} // namespace com_world
