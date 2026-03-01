#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Dumping/AssetDumpingContext.h"
#include "Game/T6/T6.h"


namespace material
{
    class JsonDumperT6 final : public AbstractAssetDumper<T6::AssetMaterial>
    {
    public:
        void Dump(AssetDumpingContext& context) override;
        
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<T6::AssetMaterial::Type>& asset) override;
    };
} // namespace material
