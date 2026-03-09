#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/T6/T6.h"

namespace xanim
{
    class DumperT6 final : public AbstractAssetDumper<T6::AssetXAnim>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<T6::AssetXAnim::Type>& asset) override;
    };
} // namespace xanim
