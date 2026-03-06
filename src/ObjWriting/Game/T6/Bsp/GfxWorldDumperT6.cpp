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
            mapFileDumper.WriteKeyValue("angles", "0.0 0.0 0.0");
            mapFileDumper.WriteKeyValue("classname", "reflection_probe");

            mapFileDumper.EndEntity();
        }
    }

    void DumpStaticModels(AssetDumpingContext& context, const XAssetInfo<AssetGfxWorld::Type>& asset)
    {
        const auto* world = asset.Asset();
        const auto assetFile = context.OpenAssetFile(bsp_common::gfx_world::GetFileNameForStaticModels(asset.m_name));

        if (!assetFile)
            return;

        MapFileDumper mapFileDumper(*assetFile);
        mapFileDumper.Init();

        auto& stream = *assetFile;

        for (unsigned int i = 0; i < world->dpvs.smodelCount; ++i)
        {
            const auto& smodel = world->dpvs.smodelDrawInsts[i];
            const auto& origin = smodel.placement.origin;
            const auto& axis = smodel.placement.axis;
            const auto& scale = smodel.placement.scale;

            mapFileDumper.BeginEntity();

            // Could probably be done better....
            vec4_t matrix[3] = {};
            matrix[0].v[0] = axis[0].x;
            matrix[0].v[1] = axis[0].y;
            matrix[0].v[2] = axis[0].z;
            matrix[1].v[0] = axis[1].x;
            matrix[1].v[1] = axis[1].y;
            matrix[1].v[2] = axis[1].z;
            matrix[2].v[0] = axis[2].x;
            matrix[2].v[1] = axis[2].y;
            matrix[2].v[2] = axis[2].z;

            vec3_t angles;
            Common::ToEulerAnglesDeg(matrix, &angles);

            mapFileDumper.WriteKeyValue("model", smodel.model->name);
            mapFileDumper.WriteKeyValue("modelscale", std::format("{:.1f}", scale));
            mapFileDumper.WriteKeyValue("origin", std::format("{:.1f} {:.1f} {:.1f}", origin.x, origin.y, origin.z));
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
        DumpReflectionProbes(context, asset);
        DumpStaticModels(context, asset);
    }
} // namespace gfx_world
