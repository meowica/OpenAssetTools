#include "FontJsonDumperT6.h"

#include "Font/FontCommon.h"
#include "Game/T6/CommonT6.h"

#include <nlohmann/json.hpp>

using namespace nlohmann;
using namespace T6;

namespace
{
    void DumpFont(std::ostream& stream, const Font_s& font)
    {
        json jRoot;

        jRoot["$schema"] = "http://openassettools.dev/schema/font.v1.json";
        jRoot["_type"] = "font";
        jRoot["_version"] = 1;
        jRoot["_game"] = "t6";

        jRoot["name"] = font.fontName;
        jRoot["pixelHeight"] = font.pixelHeight;
        jRoot["isScalingAllowed"] = font.isScalingAllowed;
        jRoot["glyphCount"] = font.glyphCount;
        jRoot["kerningPairsCount"] = font.kerningPairsCount;

        jRoot["material"] = font.material->info.name;
        jRoot["glowMaterial"] = font.glowMaterial->info.name;

        json jGlyphs = json::array();

        for (int i = 0; i < font.glyphCount; ++i)
        {
            const Glyph& g = font.glyphs[i];

            json jGlyph;
            jGlyph["letter"] = g.letter;
            jGlyph["char"] = (g.letter >= 32 && g.letter <= 126) ? std::string(1, static_cast<char>(g.letter)) : "";

            jGlyph["x0"] = g.x0;
            jGlyph["y0"] = g.y0;
            jGlyph["dx"] = g.dx;
            jGlyph["pixelWidth"] = g.pixelWidth;
            jGlyph["pixelHeight"] = g.pixelHeight;

            jGlyph["s0"] = g.s0;
            jGlyph["t0"] = g.t0;
            jGlyph["s1"] = g.s1;
            jGlyph["t1"] = g.t1;

            jGlyphs.push_back(jGlyph);
        }

        jRoot["glyphs"] = std::move(jGlyphs);

        json jKerning = json::array();

        for (int i = 0; i < font.kerningPairsCount; ++i)
        {
            const KerningPairs& k = font.kerningPairs[i];

            json jPair;
            jPair["first"] = k.wFirst;
            jPair["second"] = k.wSecond;
            jPair["amount"] = k.iKernAmount;

            jKerning.push_back(jPair);
        }

        jRoot["kerningPairs"] = std::move(jKerning);

        stream << std::setw(4) << jRoot << "\n";
    }
} // namespace

namespace font
{
    JsonDumperT6::JsonDumperT6(const AssetPool<AssetFont::Type>& pool)
        : AbstractAssetDumper(pool)
    {
    }

    void JsonDumperT6::DumpAsset(AssetDumpingContext& context, const XAssetInfo<AssetFont::Type>& asset)
    {
        const auto assetFile = context.OpenAssetFile(GetJsonFileNameForAsset(asset.m_name));

        if (!assetFile)
            return;

        DumpFont(*assetFile, *asset.Asset());
    }
} // namespace font
