#include "ComWorldDumperT6.h"

#include "Dumping/MapFile/MapFileDumper.h"

#include <format>
#include <algorithm>

using namespace T6;

namespace
{
    void DumpPrimaryLights(MapFileDumper& mapFileDumper, const ComWorld* world)
    {
        // TODO: ...this whole thing lol

        for (auto i = 0u; i < world->primaryLightCount; ++i)
        {
            const auto& pLight = world->primaryLights[i];

            const float yaw = atan2f(pLight.dir.y, pLight.dir.x) * (180.0f / M_PI);
            const float pitch = atan2f(-pLight.dir.z, sqrtf(pLight.dir.x * pLight.dir.x +
                                                            pLight.dir.y * pLight.dir.y)) * (180.0f / M_PI);

            const float fovOuter = acosf(std::clamp(pLight.cosHalfFovOuter, -1.0f, 1.0f)) * (360.0f / M_PI);
            const float fovInner = acosf(std::clamp(pLight.cosHalfFovInner, -1.0f, 1.0f)) * (360.0f / M_PI);

            const float cutOn = pLight.falloff.x;
            const float range = pLight.radius - cutOn;
            const float nearEdge = (range > 0.001f) ? (pLight.falloff.z - cutOn) / range : 0.0f;
            const float farEdge = (range > 0.001f) ? (pLight.falloff.w - cutOn) / range : 1.0f;

            mapFileDumper.BeginEntity();

            switch (pLight.type)
            {
            case 0:
                mapFileDumper.WriteKeyValue("type", "omni");
                break;

            case 1:
                mapFileDumper.WriteKeyValue("type", "point");
                break;

            default:
                mapFileDumper.WriteKeyValue("type", "spot");
                break;
            }

            mapFileDumper.WriteKeyValue("origin",
                std::format(
                    "{:.0f} {:.0f} {:.0f}",
                    pLight.origin.x,
                    pLight.origin.y,
                    pLight.origin.z));
            mapFileDumper.WriteKeyValue("angles",
                std::format(
                    "{:.1f} {:.1f} {:.1f}",
                    pitch,
                    yaw,
                    0.0f));

            const float maxChannel = std::max({pLight.color.x, pLight.color.y, pLight.color.z});
            const float intensity = maxChannel > 0.0f ? maxChannel : 1.0f;
            const float r = pLight.color.x / intensity;
            const float g = pLight.color.y / intensity;
            const float b = pLight.color.z / intensity;
            mapFileDumper.WriteKeyValue("_color", std::format("{:.6f} {:.6f} {:.6f}", r, g, b));

            mapFileDumper.WriteKeyValue("intensity", std::format("{:.4f}", intensity));
            mapFileDumper.WriteKeyValue("radius", std::format("{:.0f}", pLight.radius));
            mapFileDumper.WriteKeyValue("roundness", std::format("{:.1f}", pLight.roundness));
            mapFileDumper.WriteKeyValue("exponent", std::to_string(static_cast<int>(pLight.exponent)));
            mapFileDumper.WriteKeyValue("priority", std::to_string(static_cast<int>(pLight.priority)));
            mapFileDumper.WriteKeyValue("falloffdistance", std::format("{:.0f}", pLight.dAttenuation));
            mapFileDumper.WriteKeyValue("mipdistance", std::format("{:.1f}", pLight.mipDistance));

            mapFileDumper.WriteKeyValue("cut_on", std::format("{:.4f}", cutOn));
            mapFileDumper.WriteKeyValue("near_edge", std::format("{:.4f}", nearEdge));
            mapFileDumper.WriteKeyValue("far_edge", std::format("{:.4f}", farEdge));

            mapFileDumper.WriteKeyValue("angle",
                std::format(
                    "{:.4f} {:.4f} {:.4f}",
                    pLight.angle.x,
                    pLight.angle.y,
                    pLight.angle.z));

            mapFileDumper.WriteKeyValue(
                "superellipse",
                    std::format(
                        "{:.4f} {:.4f} {:.4f} {:.4f}",
                        pLight.aAbB.x,
                        pLight.aAbB.y,
                        pLight.aAbB.z,
                        pLight.aAbB.w));

            if (pLight.type >= 2)
            {
                mapFileDumper.WriteKeyValue("fov_outer", std::format("{:.1f}", fovOuter));
                mapFileDumper.WriteKeyValue("fov_inner", std::format("{:.1f}", fovInner));
                if (pLight.defName)
                    mapFileDumper.WriteKeyValue("def", pLight.defName);
            }

            if (pLight.canUseShadowMap)
                mapFileDumper.WriteKeyValue("castshadow", "1");
            if (pLight.shadowmapVolume)
                mapFileDumper.WriteKeyValue("shadowmapvolume", "1");
            if (pLight.rotationLimit > 0.0f && pLight.rotationLimit != 1.0f)
                mapFileDumper.WriteKeyValue("maxturn", std::format("{:.0f}", pLight.rotationLimit));
            if (pLight.translationLimit > 0.0f)
                mapFileDumper.WriteKeyValue("maxmove", std::format("{:.0f}", pLight.translationLimit));
            if (pLight.cullDist > 0)
                mapFileDumper.WriteKeyValue("culldist", std::to_string(pLight.cullDist));

            if (pLight.useCookie)
            {
                mapFileDumper.WriteKeyValue("cookie", "1");
                mapFileDumper.WriteKeyValue(
                    "cookiecontrol0",
                    std::format(
                        "{:.4f} {:.4f} {:.4f} {:.4f}",
                        pLight.cookieControl0.x,
                        pLight.cookieControl0.y,
                        pLight.cookieControl0.z,
                        pLight.cookieControl0.w));
                mapFileDumper.WriteKeyValue(
                    "cookiecontrol1",
                    std::format(
                        "{:.4f} {:.4f} {:.4f} {:.4f}",
                        pLight.cookieControl1.x,
                        pLight.cookieControl1.y,
                        pLight.cookieControl1.z,
                        pLight.cookieControl1.w));
                mapFileDumper.WriteKeyValue(
                    "cookiecontrol2",
                    std::format(
                        "{:.4f} {:.4f} {:.4f} {:.4f}",
                        pLight.cookieControl2.x,
                        pLight.cookieControl2.y,
                        pLight.cookieControl2.z,
                        pLight.cookieControl2.w));
            }

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
