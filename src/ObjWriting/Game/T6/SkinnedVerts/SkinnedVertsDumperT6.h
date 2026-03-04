#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/T6/T6.h"

namespace skinned_verts
{
    class DumperT6 final : public AbstractAssetDumper<T6::AssetSkinnedVerts>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<T6::AssetSkinnedVerts::Type>& asset) override;
    };
} // namespace skinned_verts
