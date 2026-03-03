#include "ClipMapDumperT6.h"

#include "Bsp/BSPCommon.h"

#include <format>

using namespace T6;

namespace clip_map
{
    void DumperT6::DumpAsset(AssetDumpingContext& context, const XAssetInfo<AssetClipMapPvs::Type>& asset)
    {
        const auto* clipMap = asset.Asset();
        const auto assetFile = context.OpenAssetFile(bsp_common::clip_map::GetFileNameForStaticModels(asset.m_name));

        if (!assetFile)
            return;

        auto& stream = *assetFile;

        for (unsigned int i = 0; i < clipMap->numStaticModels; ++i)
        {
            const auto& model = clipMap->staticModelList[i];

            const auto& org = model.origin;

            stream << "{\n";
            stream << "\"model\" \"" << model.xmodel->name << "\"\n";
            stream << "\"origin\" \"" << org.x << " " << org.y << " " << org.z << "\"\n";
            stream << "\"classname\" \"misc_model\"\n";
            stream << "}\n";
        }
    }
} // namespace clip_map
