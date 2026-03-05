#pragma once

#include "IGame.h"
#include "Zone/ZoneTypes.h"

#include <cstdint>
#include <optional>

enum class CommonAssetType : std::uint8_t
{
    PHYS_PRESET,
    XANIM,
    XMODEL,
    MATERIAL,
    TECHNIQUE_SET,
    IMAGE,
    SOUND,
    CLIP_MAP,
    COM_WORLD,
    GAME_WORLD_SP,
    GAME_WORLD_MP,
    MAP_ENTS,
    GFX_WORLD,
    LIGHT_DEF,
    UI_MAP,
    FONT,
    MENU_LIST,
    MENU,
    LOCALIZE_ENTRY,
    WEAPON,
    SOUND_DRIVER_GLOBALS,
    FX,
    IMPACT_FX,
    AI_TYPE,
    MP_TYPE,
    CHARACTER,
    XMODEL_ALIAS,
    RAW_FILE,
    STRING_TABLE,
    XMODEL_PIECES,
    LEADERBOARD,
    TRACER,
    VEHICLE,
    ADDON_MAP_ENTS,
    ATTACHMENT,
    SCRIPT,
    PHYS_CONSTRAINTS,
    DESTRUCTIBLE_DEF,
    SOUND_PATCH,
    WEAPON_DEF,
    WEAPON_VARIANT,
    MP_BODY,
    MP_HEAD,
    XGLOBALS,
    DDL,
    GLASSES,
    EMBLEM_SET,
    FONT_ICON,
    WEAPON_FULL,
    ATTACHMENT_UNIQUE,
    WEAPON_CAMO,
    KEY_VALUE_PAIRS,
    MEMORY_BLOCK,
    SKINNED_VERTS,
    QDB,
    SLUG,
    FOOTSTEP_TABLE,
    FOOTSTEP_FX_TABLE,
    ZBARRIER,

    COUNT,
};

class ICommonAssetTypeMapper
{
public:
    [[nodiscard]] virtual CommonAssetType GameToCommonAssetType(asset_type_t gameAssetType) const = 0;
    [[nodiscard]] virtual std::optional<asset_type_t> CommonToGameAssetType(CommonAssetType commonAssetType) const = 0;

    static ICommonAssetTypeMapper* GetCommonAssetMapperByGame(GameId gameId);

protected:
    ICommonAssetTypeMapper() = default;
    virtual ~ICommonAssetTypeMapper() = default;
    ICommonAssetTypeMapper(const ICommonAssetTypeMapper& other) = default;
    ICommonAssetTypeMapper(ICommonAssetTypeMapper&& other) noexcept = default;
    ICommonAssetTypeMapper& operator=(const ICommonAssetTypeMapper& other) = default;
    ICommonAssetTypeMapper& operator=(ICommonAssetTypeMapper&& other) noexcept = default;
};
