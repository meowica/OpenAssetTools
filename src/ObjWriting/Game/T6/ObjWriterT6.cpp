#include "ObjWriterT6.h"

#include "Bsp/ClipMapDumperT6.h"
#include "Bsp/ClipMapPvsDumperT6.h"
#include "Bsp/ComWorldDumperT6.h"
#include "Bsp/GameWorldMpDumperT6.h"
#include "Bsp/GameWorldSpDumperT6.h"
#include "Bsp/GfxWorldDumperT6.h"
#include "Bsp/MapEntsDumperT6.h"
#include "Bsp/AddonMapEntsDumperT6.h"
#include "Font/FontDumperT6.h"
#include "FontIcon/FontIconDumperT6.h"
#include "Material/MaterialJsonDumperT6.h"
#include "XModel/XModelDumperT6.h"
#include "Image/ImageDumperT6.h"
#include "Leaderboard/LeaderboardJsonDumperT6.h"
#include "Localize/LocalizeDumperT6.h"
#include "LightDef/LightDefDumperT6.h"
#include "Menu/MenuListDumperT6.h"
#include "Menu/MenuDumperT6.h"
#include "Menu/MenuWriterT6.h"
#include "ObjWriting.h"
#include "PhysConstraints/PhysConstraintsInfoStringDumperT6.h"
#include "PhysPreset/PhysPresetInfoStringDumperT6.h"
#include "Qdb/QdbDumperT6.h"
#include "RawFile/RawFileDumperT6.h"
#include "Script/ScriptDumperT6.h"
#include "Slug/SlugDumperT6.h"
#include "Sound/SndBankDumperT6.h"
#include "Sound/SndDriverGlobalsDumperT6.h"
#include "StringTable/StringTableDumperT6.h"
#include "Techset/TechsetDumperT6.h"
#include "Tracer/TracerDumperT6.h"
#include "Vehicle/VehicleDumperT6.h"
#include "Weapon/AttachmentDumperT6.h"
#include "Weapon/AttachmentUniqueDumperT6.h"
#include "Weapon/CamoJsonDumperT6.h"
#include "Weapon/WeaponDumperT6.h"
#include "XAnim/XAnimDumperT6.h"
#include "ZBarrier/ZBarrierDumperT6.h"

using namespace T6;

void ObjWriter::RegisterAssetDumpers(AssetDumpingContext& context)
{
    RegisterAssetDumper(std::make_unique<phys_preset::InfoStringDumperT6>());
    RegisterAssetDumper(std::make_unique<phys_constraints::InfoStringDumperT6>());
    // RegisterAssetDumper(std::make_unique<destructible::InfoStringDumperT6>());
    RegisterAssetDumper(std::make_unique<xanim::DumperT6>());
    RegisterAssetDumper(std::make_unique<xmodel::DumperT6>());
    RegisterAssetDumper(std::make_unique<material::JsonDumperT6>());
    RegisterAssetDumper(std::make_unique<techset::DumperT6>(
#ifdef TECHSET_DEBUG
        true
#else
        false
#endif
        ));
    RegisterAssetDumper(std::make_unique<image::DumperT6>());
    RegisterAssetDumper(std::make_unique<sound::SndBankDumperT6>());
    // RegisterAssetDumper(std::make_unique<sound_patch::DumperT6>());
    RegisterAssetDumper(std::make_unique<clip_map::DumperT6>());
    RegisterAssetDumper(std::make_unique<clip_map_pvs::DumperT6>());
    RegisterAssetDumper(std::make_unique<com_world::DumperT6>());
    RegisterAssetDumper(std::make_unique<game_world_sp::DumperT6>());
    RegisterAssetDumper(std::make_unique<game_world_mp::DumperT6>());
    RegisterAssetDumper(std::make_unique<map_ents::DumperT6>());
    RegisterAssetDumper(std::make_unique<gfx_world::DumperT6>());
    RegisterAssetDumper(light_def::CreateDumperT6());
    RegisterAssetDumper(font::CreateDumperT6());
    RegisterAssetDumper(font_icon::CreateDumperT6());
    RegisterAssetDumper(std::make_unique<menu::MenuListDumperT6>());
    RegisterAssetDumper(std::make_unique<menu::MenuDumperT6>());
    RegisterAssetDumper(std::make_unique<localize::DumperT6>());
    RegisterAssetDumper(std::make_unique<weapon::DumperT6>());
    RegisterAssetDumper(std::make_unique<attachment::DumperT6>());
    RegisterAssetDumper(std::make_unique<attachment_unique::DumperT6>());
    RegisterAssetDumper(std::make_unique<camo::JsonDumperT6>());
    RegisterAssetDumper(std::make_unique<sound::SndDriverGlobalsDumperT6>());
    // RegisterAssetDumper(std::make_unique<fx::DumperT6>());
    // RegisterAssetDumper(std::make_unique<impact_fx::DumperT6>());
    RegisterAssetDumper(std::make_unique<raw_file::DumperT6>());
    RegisterAssetDumper(std::make_unique<string_table::DumperT6>());
    RegisterAssetDumper(std::make_unique<leaderboard::JsonDumperT6>());
    // RegisterAssetDumper(std::make_unique<xglobals::DumperT6>());
    // RegisterAssetDumper(std::make_unique<ddl::DumperT6>());
    // RegisterAssetDumper(std::make_unique<glasses::DumperT6>());
    // RegisterAssetDumper(std::make_unique<emblem::DumperT6>());
    RegisterAssetDumper(std::make_unique<script::DumperT6>());
    // RegisterAssetDumper(std::make_unique<key_value_pairs::DumperT6>());
    RegisterAssetDumper(std::make_unique<vehicle::DumperT6>());
    // RegisterAssetDumper(std::make_unique<memory::DumperT6>());
    RegisterAssetDumper(std::make_unique<addon_map_ents::DumperT6>());
    RegisterAssetDumper(std::make_unique<tracer::DumperT6>());
    // RegisterAssetDumper(std::make_unique<skinned_verts::DumperT6>());
    RegisterAssetDumper(std::make_unique<qdb::DumperT6>());
    RegisterAssetDumper(std::make_unique<slug::DumperT6>());
    // RegisterAssetDumper(std::make_unique<footstep::DumperT6>());
    // RegisterAssetDumper(std::make_unique<footstep_fx::DumperT6>());
    RegisterAssetDumper(std::make_unique<zbarrier::DumperT6>());
}
