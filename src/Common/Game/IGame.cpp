#include "IGame.h"

#include "T6/GameT6.h"

#include <cassert>

IGame* IGame::GetGameById(GameId)
{
    static T6::Game game{};
    return &game;
}
