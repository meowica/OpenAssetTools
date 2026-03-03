#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/T6/T6.h"

namespace gfx_world
{
    class DumperT6 final : public AbstractAssetDumper<T6::AssetGfxWorld>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<T6::AssetGfxWorld::Type>& asset) override;
    };
} // namespace gfx_world
