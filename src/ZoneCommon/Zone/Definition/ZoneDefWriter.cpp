#include "ZoneDefWriter.h"

#include "Game/T6/Zone/Definition/ZoneDefWriterT6.h"

#include <cassert>

const IZoneDefWriter* IZoneDefWriter::GetZoneDefWriterForGame(GameId)
{
    static const T6::ZoneDefWriter writer{};
    return &writer;
}

void AbstractZoneDefWriter::WriteZoneDef(std::ostream& stream, const Zone& zone, const bool useGdt) const
{
    ZoneDefinitionOutputStream out(stream);
    const auto* game = IGame::GetGameById(zone.m_game_id);

    out.WriteComment(game->GetFullName());
    out.WriteMetaData(META_DATA_KEY_GAME, game->GetShortName());
    out.EmptyLine();

    if (useGdt)
    {
        out.WriteComment("Load asset gdt files");
        out.WriteMetaData(META_DATA_KEY_GDT, zone.m_name);
        out.EmptyLine();
    }

    WriteMetaData(out, zone);
    WriteContent(out, zone);
}
