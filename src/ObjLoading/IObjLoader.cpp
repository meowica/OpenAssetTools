#include "IObjLoader.h"

#include "Game/T6/ObjLoaderT6.h"

#include <cassert>

const IObjLoader* IObjLoader::GetObjLoaderForGame(GameId game)
{
    static const T6::ObjLoader loader{};
    return &loader;
}
