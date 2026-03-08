#include "GfxWorldDumperT6.h"

#include "Dumping/MapFile/MapFileDumper.h"

#include <format>

using namespace T6;

namespace
{
    void DumpReflectionProbes(MapFileDumper& mapFileDumper, const GfxWorld* world)
    {
        const auto& worldDraw = world->draw;

        for (auto i = 0u; i < worldDraw.reflectionProbeCount; ++i)
        {
            const auto& rProbe = worldDraw.reflectionProbes[i];

            mapFileDumper.BeginEntity();
            mapFileDumper.WriteKeyValue("origin", std::format("{:.1f} {:.1f} {:.1f}", rProbe.origin.x, rProbe.origin.y, rProbe.origin.z));
            mapFileDumper.WriteKeyValue("classname", "reflection_probe");
            mapFileDumper.EndEntity();
        }
    }

    void DumpStaticModels(MapFileDumper& mapFileDumper, const GfxWorld* world)
    {
        const auto& worldDpvs = world->dpvs;

        for (auto i = 0u; i < worldDpvs.smodelCount; ++i)
        {
            const auto& sModel = worldDpvs.smodelDrawInsts[i];
            const auto& placement = sModel.placement;

            mapFileDumper.BeginEntity();

            vec4_t matrix[3] = {
                {placement.axis[0].x, placement.axis[0].y, placement.axis[0].z, 0.0f},
                {placement.axis[1].x, placement.axis[1].y, placement.axis[1].z, 0.0f},
                {placement.axis[2].x, placement.axis[2].y, placement.axis[2].z, 0.0f},
            };

            vec3_t angles;
            Common::ToEulerAnglesDeg(matrix, &angles);

            mapFileDumper.WriteKeyValue("model", sModel.model->name);
            mapFileDumper.WriteKeyValue("modelscale", std::format("{:.1f}", placement.scale));
            mapFileDumper.WriteKeyValue("origin", std::format("{:.1f} {:.1f} {:.1f}", placement.origin.x, placement.origin.y, placement.origin.z));
            mapFileDumper.WriteKeyValue("angles", std::format("{:.1f} {:.1f} {:.1f}", angles.x, angles.y, angles.z));
            mapFileDumper.WriteKeyValue("classname", "misc_model");

            mapFileDumper.EndEntity();
        }
    }
} // namespace

namespace gfx_world
{
    void DumperT6::DumpAsset(AssetDumpingContext& context, const XAssetInfo<AssetGfxWorld::Type>& asset)
    {
        auto* mapDumper = context.GetZoneAssetDumperState<MapFileDumperState>()->GetOrCreateDumper(context, asset.m_name);
        if (!mapDumper)
            return;

        DumpReflectionProbes(*mapDumper, asset.Asset());
        DumpStaticModels(*mapDumper, asset.Asset());
    }
} // namespace gfx_world
