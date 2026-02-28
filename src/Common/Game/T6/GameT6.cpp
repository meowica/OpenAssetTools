#include "GameT6.h"

#include "T6.h"

#include <algorithm>

using namespace T6;

namespace
{
    constexpr const char* ASSET_TYPE_NAMES[ASSET_TYPE_COUNT]
    {
        "xmodelpieces",
        "physpreset",
        "physconstraints",
        "destructible",
        "xanim",
        "xmodel",
        "material",
        "techset",
        "image",
        "sound",
        "sound_patch",
        "col_map_sp", // is this correct?
        "col_map_mp",
        "com_map",
        "game_map_sp",
        "game_map_mp",
        "map_ents",
        "gfx_map",
        "lightdef",
        "ui_map",
        "font",
        "fonticon",
        "menufile",
        "menu",
        "localize",
        "weapon",
        "weapondef",
        "weaponvariant",
        "weaponfull",
        "attachment",
        "attachmentunique",
        "camo",
        "snddriverglobals",
        "fx",
        "impactfx",
        "aitype",
        "mptype",
        "mpbody",
        "mphead",
        "character",
        "xmodelalias",
        "rawfile",
        "stringtable",
        "leaderboard",
        "xGlobals",
        "ddl",
        "glasses",
        "emblemSet",
        "script",
        "keyvaluepairs",
        "vehicle",
        "memory",
        "addon_map_ents",
        "tracer",
        "skin",
        "qdb",
        "slug",
        "footstep",
        "footstepfx",
        "zbarrier",
    };

    constexpr const char* SUB_ASSET_TYPE_NAMES[SUB_ASSET_TYPE_COUNT]
    {
        "technique",
        "vertexshader",
        "pixelshader",
    };
} // namespace

namespace T6
{
    GameId Game::GetId() const
    {
        return GameId::T6;
    }

    const std::string& Game::GetFullName() const
    {
        static std::string fullName = "Call Of Duty: Black Ops II";
        return fullName;
    }

    const std::string& Game::GetShortName() const
    {
        static std::string shortName = "T6";
        return shortName;
    }

    const std::vector<GameLanguagePrefix>& Game::GetLanguagePrefixes() const
    {
        static std::vector<GameLanguagePrefix> prefixes
        {
            {GameLanguage::LANGUAGE_ENGLISH,         "en_"},
            {GameLanguage::LANGUAGE_FRENCH,          "fr_"},
            {GameLanguage::LANGUAGE_FRENCH_CAN,      "fc_"},
            {GameLanguage::LANGUAGE_GERMAN,          "ge_"},
            {GameLanguage::LANGUAGE_AUSTRIAN,        "as_"},
            {GameLanguage::LANGUAGE_ITALIAN,         "it_"},
            {GameLanguage::LANGUAGE_SPANISH,         "sp_"},
            {GameLanguage::LANGUAGE_BRITISH,         "br_"},
            {GameLanguage::LANGUAGE_RUSSIAN,         "ru_"},
            {GameLanguage::LANGUAGE_POLISH,          "po_"},
            {GameLanguage::LANGUAGE_KOREAN,          "ko_"},
            {GameLanguage::LANGUAGE_JAPANESE,        "ja_"},
            {GameLanguage::LANGUAGE_CZECH,           "cz_"},
            {GameLanguage::LANGUAGE_FULL_JAPANESE,   "fj_"},
            {GameLanguage::LANGUAGE_PORTUGUESE,      "bp_"},
            {GameLanguage::LANGUAGE_MEXICAN_SPANISH, "ms_"},
        };
        return prefixes;
    }

    asset_type_t Game::GetAssetTypeCount() const
    {
        return ASSET_TYPE_COUNT;
    }

    std::optional<const char*> Game::GetAssetTypeName(const asset_type_t assetType) const
    {
        if (assetType < std::extent_v<decltype(ASSET_TYPE_NAMES)>)
            return ASSET_TYPE_NAMES[assetType];
        return std::nullopt;
    }

    asset_type_t Game::GetSubAssetTypeCount() const
    {
        return SUB_ASSET_TYPE_COUNT;
    }

    std::optional<const char*> Game::GetSubAssetTypeName(const asset_type_t subAssetType) const
    {
        if (subAssetType < std::extent_v<decltype(SUB_ASSET_TYPE_NAMES)>)
            return SUB_ASSET_TYPE_NAMES[subAssetType];
        return std::nullopt;
    }
} // namespace T6
