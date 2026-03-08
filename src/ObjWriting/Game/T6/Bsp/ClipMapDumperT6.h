#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/T6/T6.h"
#include "Game/T6/CommonT6.h"

namespace clip_map
{
    class DumperT6 final : public AbstractAssetDumper<T6::AssetClipMap>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<T6::AssetClipMap::Type>& asset) override;
    };
} // namespace clip_map
