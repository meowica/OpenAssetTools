#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/T6/T6.h"

namespace light_def
{
    class JsonDumperT6 final : public AbstractAssetDumper<T6::AssetLightDef>
    {
    public:
        explicit JsonDumperT6(const AssetPool<T6::AssetLightDef::Type>& pool);

    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<T6::AssetLightDef::Type>& asset) override;
    };
} // namespace light_def
