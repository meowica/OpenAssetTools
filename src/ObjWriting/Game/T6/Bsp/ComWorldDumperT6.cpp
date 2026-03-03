#include "ComWorldDumperT6.h"

#include "Dumping/MapFile/MapFileDumper.h"
#include "Bsp/BSPCommon.h"

#include <format>

using namespace T6;

namespace com_world
{
    void DumperT6::DumpAsset(AssetDumpingContext& context, const XAssetInfo<AssetComWorld::Type>& asset)
    {
        const auto* comWorld = asset.Asset();
        const auto assetFile = context.OpenAssetFile(bsp_common::com_world::GetFileNameForPrimaryLights(asset.m_name));

        if (!assetFile)
            return;

        auto& stream = *assetFile;

        for (unsigned int i = 0; i < comWorld->primaryLightCount; ++i)
        {
            const auto& light = comWorld->primaryLights[i];

            stream << "{\n";
            stream << "\"origin\" \"" << light.origin.x << " " << light.origin.y << " " << light.origin.z << "\"\n";
            stream << "\"def\" \"" << (light.defName ? light.defName : "unknown") << "\"\n";
            stream << "\"radius\" \"" << light.radius << "\"\n";
            stream << "\"_color\" \"" << light.color.x << " " << light.color.y << " " << light.color.z << "\"\n";
            stream << "\"intensity\" \"" << 1.0f << "\"\n";
            stream << "\"classname\" \"light\"\n";
            stream << "}\n";
        }
    }
} // namespace com_world
