#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/T6/T6.h"

namespace game_world_sp
{
    class DumperT6 final : public AbstractAssetDumper<T6::AssetGameWorldSp>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<T6::AssetGameWorldSp::Type>& asset) override;
    };
} // namespace game_world_sp
