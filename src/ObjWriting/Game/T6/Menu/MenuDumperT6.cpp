#include "MenuDumperT6.h"

#include "Game/T6/GameAssetPoolT6.h"
#include "MenuWriterT6.h"
#include "ObjWriting.h"

#include <filesystem>
#include <format>
#include <string>

namespace fs = std::filesystem;

using namespace T6;

namespace
{
    const MenuList* GetParentMenuList(const XAssetInfo<menuDef_t>& asset)
    {
        const auto* menu = asset.Asset();
        const auto* gameAssetPool = dynamic_cast<GameAssetPoolT6*>(asset.m_zone->m_pools.get());
        for (const auto* menuList : *gameAssetPool->m_menu_list)
        {
            const auto* menuListAsset = menuList->Asset();

            for (auto menuIndex = 0; menuIndex < menuListAsset->menuCount; menuIndex++)
            {
                if (menuListAsset->menus[menuIndex] == menu)
                    return menuListAsset;
            }
        }

        return nullptr;
    }

    std::string GetPathForMenu(const XAssetInfo<menuDef_t>& asset)
    {
        const auto* list = GetParentMenuList(asset);

        if (!list)
            return std::format("ui_mp/{}.menu", asset.Asset()->window.name);

        const fs::path p(list->name);
        std::string parentPath;
        if (p.has_parent_path())
            parentPath = p.parent_path().string() + "/";

        return std::format("{}{}.menu", parentPath, asset.Asset()->window.name);
    }
} // namespace

namespace menu
{
    MenuDumperT6::MenuDumperT6(const AssetPool<AssetMenu::Type>& pool)
        : AbstractAssetDumper(pool)
    {
    }

    void MenuDumperT6::DumpAsset(AssetDumpingContext& context, const XAssetInfo<AssetMenu::Type>& asset)
    {
        const auto* menu = asset.Asset();
        const auto menuFilePath = GetPathForMenu(asset);

        if (ObjWriting::ShouldHandleAssetType(ASSET_TYPE_MENULIST))
        {
            // Don't dump menu file separately if the name matches the menu list
            const auto* menuListParent = GetParentMenuList(asset);
            if (menuListParent && menuFilePath == menuListParent->name)
                return;
        }

        const auto assetFile = context.OpenAssetFile(menuFilePath);

        if (!assetFile)
            return;

        auto menuWriter = CreateMenuWriterT6(*assetFile);

        menuWriter->Start();
        menuWriter->WriteMenu(*menu);
        menuWriter->End();
    }
} // namespace menu
