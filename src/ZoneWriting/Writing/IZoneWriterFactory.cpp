#include "IZoneWriterFactory.h"

#include "Game/T6/ZoneWriterFactoryT6.h"

#include <cassert>

const IZoneWriterFactory* IZoneWriterFactory::GetZoneWriterFactoryForGame(GameId)
{
    static const T6::ZoneWriterFactory factory{};
    return &factory;
}
