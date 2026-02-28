#include "MenuConversionZoneStateT6.h"

#include <cstring>

using namespace T6;

MenuConversionZoneState::MenuConversionZoneState()
    : m_zone(nullptr)
{
}

void MenuConversionZoneState::Inject(ZoneAssetCreationInjection& inject)
{
    auto& memory = inject.m_zone.Memory();

    m_zone = &inject.m_zone;
}

const char* MenuConversionZoneState::AddString(const std::string& str)
{
    const auto foundString = m_strings_by_value.find(str);

    if (foundString != m_strings_by_value.end())
        return foundString->second;

    auto& memory = m_zone->Memory();
    const auto* strDuped = memory.Dup(str.c_str());

    m_strings.push_back(strDuped);
    m_strings_by_value.emplace(std::make_pair(str, strDuped));

    return strDuped;
}

void MenuConversionZoneState::AddLoadedFile(std::string loadedFileName, std::vector<XAssetInfo<menuDef_t>*> menusOfFile)
{
    m_menus_by_filename.emplace(std::make_pair(std::move(loadedFileName), std::move(menusOfFile)));
}
