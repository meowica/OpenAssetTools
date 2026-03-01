#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/T6/T6.h"


namespace xmodel
{
    class DumperT6 final : public AbstractAssetDumper<T6::AssetXModel>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<T6::AssetXModel::Type>& asset) override;
    };
} // namespace xmodel
