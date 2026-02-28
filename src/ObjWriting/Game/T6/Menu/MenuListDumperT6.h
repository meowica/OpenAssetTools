#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/T6/T6.h"

namespace menu
{
    class MenuListDumperT6 final : public AbstractAssetDumper<T6::AssetMenuList>
    {
    public:
        explicit MenuListDumperT6(const AssetPool<T6::AssetMenuList::Type>& pool);

    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<T6::AssetMenuList::Type>& asset) override;
    };
} // namespace menu
