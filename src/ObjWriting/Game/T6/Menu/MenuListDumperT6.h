#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/T6/T6.h"
#include "Menu/MenuDumpingZoneState.h"

namespace menu
{
    void CreateDumpingStateForMenuListT6(MenuDumpingZoneState* zoneState, const T6::MenuList* menuList);

    class MenuListDumperT6 final : public AbstractAssetDumper<T6::AssetMenuList>
    {
    public:
        void Dump(AssetDumpingContext& context) override;

    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<T6::AssetMenuList::Type>& asset) override;
    };
} // namespace menu
