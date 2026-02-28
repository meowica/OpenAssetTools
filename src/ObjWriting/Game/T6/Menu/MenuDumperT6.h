#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/T6/T6.h"

namespace menu
{
    class MenuDumperT6 final : public AbstractAssetDumper<T6::AssetMenu>
    {
    protected:
        void DumpAsset(AssetDumpingContext& context, const XAssetInfo<T6::AssetMenu::Type>& asset) override;
    };
} // namespace menu
