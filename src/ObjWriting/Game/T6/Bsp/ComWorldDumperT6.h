#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/T6/T6.h"
#include "Game/T6/CommonT6.h"

namespace com_world
{
    class DumperT6 final : public AbstractAssetDumper<T6::AssetComWorld>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<T6::AssetComWorld::Type>& asset) override;
    };
} // namespace com_world
