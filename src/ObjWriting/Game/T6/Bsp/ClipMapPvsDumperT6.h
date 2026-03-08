#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/T6/T6.h"
#include "Game/T6/CommonT6.h"

namespace clip_map_pvs
{
    class DumperT6 final : public AbstractAssetDumper<T6::AssetClipMapPvs>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<T6::AssetClipMapPvs::Type>& asset) override;
    };
} // namespace clip_map_pvs
