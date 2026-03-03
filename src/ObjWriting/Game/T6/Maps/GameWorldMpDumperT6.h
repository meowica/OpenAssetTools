#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/T6/T6.h"

namespace game_world_mp
{
    class DumperT6 final : public AbstractAssetDumper<T6::AssetGameWorldMp>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<T6::AssetGameWorldMp::Type>& asset) override;
    };
} // namespace game_world_mp
