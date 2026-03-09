#include "DestructibleInfoStringDumperT6.h"

#include "Game/T6/InfoString/InfoStringFromStructConverter.h"
#include "Game/T6/ObjConstantsT6.h"
#include "Game/T6/Destructible/DestructibleFields.h"
#include "Destructible/DestructibleCommon.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <type_traits>

using namespace T6;

namespace
{
    class InfoStringFromDestructiblePieceConverter final : public InfoStringFromStructConverter
    {
    protected:
        void FillFromExtensionField(const cspField_t& field) override
        {
            assert(false);
        }

    public:
        InfoStringFromDestructiblePieceConverter(const DestructiblePiece* structure,
                                                 const cspField_t* fields,
                                                 const size_t fieldCount,
                                                 std::function<std::string(scr_string_t)> scriptStringValueCallback)
            : InfoStringFromStructConverter(structure, fields, fieldCount, std::move(scriptStringValueCallback))
        {
        }
    };

    InfoString CreateDestructiblePieceInfoString(const DestructiblePiece* piece, const XAssetInfo<DestructibleDef>& asset)
    {
        InfoStringFromDestructiblePieceConverter converter(piece,
                                                           destructible_piece_fields,
                                                           std::extent_v<decltype(destructible_piece_fields)>,
                                                           [&asset](const scr_string_t scrStr) -> std::string
                                                           {
                                                               assert(scrStr < asset.m_zone->m_script_strings.Count());
                                                               if (scrStr >= asset.m_zone->m_script_strings.Count())
                                                                   return "";
                                                               return asset.m_zone->m_script_strings[scrStr];
                                                           });
        return converter.Convert();
    }

    class InfoStringFromDestructibleDefConverter final : public InfoStringFromStructConverter
    {
    protected:
        void FillFromExtensionField(const cspField_t& field) override
        {
            assert(false);
        }

    public:
        InfoStringFromDestructibleDefConverter(const DestructibleDef* structure,
                                               const cspField_t* fields,
                                               const size_t fieldCount,
                                               std::function<std::string(scr_string_t)> scriptStringValueCallback)
            : InfoStringFromStructConverter(structure, fields, fieldCount, std::move(scriptStringValueCallback))
        {
        }
    };

    InfoString CreateInfoString(const XAssetInfo<DestructibleDef>& asset)
    {
        InfoStringFromDestructibleDefConverter converter(asset.Asset(),
                                                         destructible_def_fields,
                                                         std::extent_v<decltype(destructible_def_fields)>,
                                                         [asset](const scr_string_t scrStr) -> std::string
                                                         {
                                                             assert(scrStr < asset.m_zone->m_script_strings.Count());
                                                             if (scrStr >= asset.m_zone->m_script_strings.Count())
                                                                return "";

                                                             return asset.m_zone->m_script_strings[scrStr];
                                                         });

        return converter.Convert();
    }
} // namespace

namespace destructible
{
    void InfoStringDumperT6::DumpAsset(AssetDumpingContext& context, const XAssetInfo<AssetDestructibleDef::Type>& asset)
    {
        const auto* dDef = asset.Asset();

        // Only dump raw when no gdt available
        if (context.m_gdt)
        {
            const auto infoString = CreateInfoString(asset);
            GdtEntry gdtEntry(asset.m_name, ObjConstants::GDF_FILENAME_DESTRUCTIBLE_DEF);
            infoString.ToGdtProperties(ObjConstants::INFO_STRING_PREFIX_DESTRUCTIBLE_DEF, gdtEntry);
            context.m_gdt->WriteEntry(gdtEntry);
        }
        else
        {
            const auto assetFile = context.OpenAssetFile(GetFileNameForDestructibleDef(asset.m_name));
            if (!assetFile)
                return;

            auto& stream = *assetFile;
            const auto infoString = CreateInfoString(asset);
            const auto stringValue = infoString.ToString(ObjConstants::INFO_STRING_PREFIX_DESTRUCTIBLE_DEF);
            stream.write(stringValue.c_str(), stringValue.size());
        }

        for (auto i = 0; i < dDef->numPieces; i++)
        {
            const auto* dPiece = &dDef->pieces[i];

            // Only dump raw when no gdt available
            if (context.m_gdt)
            {
                const auto infoString = CreateDestructiblePieceInfoString(dPiece, asset);
                GdtEntry gdtEntry(asset.m_name + "::piece_" + std::to_string(i), ObjConstants::GDF_FILENAME_DESTRUCTIBLE_PIECE);
                infoString.ToGdtProperties(ObjConstants::INFO_STRING_PREFIX_DESTRUCTIBLE_PIECE, gdtEntry);
                context.m_gdt->WriteEntry(gdtEntry);
            }
            else
            {
                const auto assetFile = context.OpenAssetFile(GetFileNameForDestructiblePiece(asset.m_name, i));
                if (!assetFile)
                    continue;

                auto& stream = *assetFile;
                const auto infoString = CreateDestructiblePieceInfoString(dPiece, asset);
                const auto stringValue = infoString.ToString(ObjConstants::INFO_STRING_PREFIX_DESTRUCTIBLE_PIECE);
                stream.write(stringValue.c_str(), stringValue.size());
            }
        }
    }
} // namespace destructible
