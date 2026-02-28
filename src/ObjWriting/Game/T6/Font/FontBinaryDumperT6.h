#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/T6/T6.h"

namespace font
{
    class BinaryDumperT6 final : public AbstractAssetDumper<T6::AssetFont>
    {
    public:
        explicit BinaryDumperT6(const AssetPool<T6::AssetFont::Type>& pool);

    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<T6::AssetFont::Type>& asset) override;
    };
} // namespace font
