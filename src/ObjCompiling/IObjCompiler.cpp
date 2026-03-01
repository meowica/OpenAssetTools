#include "IObjCompiler.h"

#include "Game/T6/ObjCompilerT6.h"

#include <cassert>

const IObjCompiler* IObjCompiler::GetObjCompilerForGame(GameId)
{
    static const T6::ObjCompiler compiler{};
    return &compiler;
}
