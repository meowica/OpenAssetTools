#include "MenuListDumperT6.h"

#include "MenuWriterT6.h"
#include "Menu/AbstractMenuWriter.h"
#include "ObjWriting.h"

#include <cassert>
#include <filesystem>
#include <format>
#include <set>
#include <sstream>

namespace fs = std::filesystem;

using namespace T6;

namespace
{
    void DumpMenus(menu::IWriterT6& menuDumper, const MenuList* menuList)
    {
        const fs::path p(menuList->name);

        std::string parentPath;
        if (p.has_parent_path())
            parentPath = p.parent_path().string() + "/";

        for (auto menuNum = 0; menuNum < menuList->menuCount; menuNum++)
        {
            const auto* menu = menuList->menus[menuNum];
            const auto* menuAssetName = menu->window.name;

            bool isReference = false;
            if (menuAssetName && menuAssetName[0] == ',')
            {
                menuAssetName = &menuAssetName[1];
                isReference = true;
            }

            std::ostringstream ss;
            ss << parentPath << menuAssetName << ".menu";

            const auto menuName = ss.str();

            // If the menu was embedded directly as menu list write its data in the menu list file
            if (!isReference && menuName == menuList->name)
                menuDumper.WriteMenu(*menu);
            else
                menuDumper.IncludeMenu(ss.str());
        }
    }
} // namespace

namespace menu
{
    MenuListDumperT6::MenuListDumperT6(const AssetPool<AssetMenuList::Type>& pool)
        : AbstractAssetDumper(pool)
    {
    }

    void MenuListDumperT6::DumpAsset(AssetDumpingContext& context, const XAssetInfo<AssetMenuList::Type>& asset)
    {
        const auto* menuList = asset.Asset();
        const auto assetFile = context.OpenAssetFile(asset.m_name);

        if (!assetFile)
            return;

        auto menuWriter = CreateMenuWriterT6(*assetFile);

        menuWriter->Start();

        DumpMenus(*menuWriter, menuList);

        menuWriter->End();
    }
} // namespace menu
