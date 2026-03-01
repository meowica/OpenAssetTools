#include "IZoneLoaderFactory.h"

#include "Game/T6/ZoneLoaderFactoryT6.h"

#include <cassert>

const IZoneLoaderFactory* IZoneLoaderFactory::GetZoneLoaderFactoryForGame(GameId)
{
    static const T6::ZoneLoaderFactory factory{};
    return &factory;
}
