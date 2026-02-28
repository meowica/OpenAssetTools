#include "LightDefJsonDumperT6.h"

#include "LightDef/LightDefCommon.h"
#include "Game/T6/CommonT6.h"
#include "Game/T6/LightDef/JsonLightDefT6.h"

#include <nlohmann/json.hpp>

using namespace nlohmann;
using namespace T6;

namespace
{
    void DumpLight(std::ostream& stream, const GfxLightDef& lightDef)
    {
        json jRoot;

        // jRoot["$schema"] = "http://openassettools.dev/schema/light-def.v1.json";
        jRoot["_type"] = "light-def";
        jRoot["_version"] = 1;
        jRoot["_game"] = "t6";

        jRoot["name"] = lightDef.name;

        json jAtten;
        jAtten["image"] = lightDef.attenuation.image->name;
        jAtten["samplerState"] = lightDef.attenuation.samplerState;
        jRoot["attenuation"] = jAtten;
        jRoot["lmapLookupStart"] = lightDef.lmapLookupStart;

        stream << std::setw(4) << jRoot << "\n";
    }
} // namespace

namespace light_def
{
    JsonDumperT6::JsonDumperT6(const AssetPool<AssetLightDef::Type>& pool)
        : AbstractAssetDumper(pool)
    {
    }

    void JsonDumperT6::DumpAsset(AssetDumpingContext& context, const XAssetInfo<AssetLightDef::Type>& asset)
    {
        const auto assetFile = context.OpenAssetFile(light_def::GetJsonFileNameForAsset(asset.m_name));

        if (!assetFile)
            return;

        DumpLight(*assetFile, *asset.Asset());
    }
} // namespace light_def
