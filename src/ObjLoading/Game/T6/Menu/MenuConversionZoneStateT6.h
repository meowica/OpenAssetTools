#pragma once

#include "Asset/IZoneAssetCreationState.h"
#include "Game/T6/T6.h"

#include <map>

namespace T6
{
    class MenuConversionZoneState final : public IZoneAssetCreationState
    {
        Zone* m_zone;

        std::map<std::string, size_t> m_dvars_by_name;

        std::vector<const char*> m_strings;
        std::map<std::string, const char*> m_strings_by_value;

    public:
        std::map<std::string, std::vector<XAssetInfo<menuDef_t>*>> m_menus_by_filename;

        MenuConversionZoneState();
        void Inject(ZoneAssetCreationInjection& inject) override;

        const char* AddString(const std::string& str);

        void AddLoadedFile(std::string loadedFileName, std::vector<XAssetInfo<menuDef_t>*> menusOfFile);
    };
} // namespace T6
