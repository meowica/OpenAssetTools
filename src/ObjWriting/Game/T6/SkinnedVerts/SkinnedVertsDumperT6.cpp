#include "SkinnedVertsDumperT6.h"

using namespace T6;

namespace skinned_verts
{
    void DumperT6::DumpAsset(AssetDumpingContext& context, const XAssetInfo<AssetSkinnedVerts::Type>& asset)
    {
        const auto* skinnedVerts = asset.Asset();
        const auto assetFile = context.OpenAssetFile(asset.m_name);

        if (!assetFile)
            return;

        auto& stream = *assetFile;
        stream.write(reinterpret_cast<const char*>(&skinnedVerts->maxSkinnedVerts), sizeof(skinnedVerts->maxSkinnedVerts));
    }
} // namespace skinned_verts
