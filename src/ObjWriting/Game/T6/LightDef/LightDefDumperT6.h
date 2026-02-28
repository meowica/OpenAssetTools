#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/T6/T6.h"

#include <memory>

namespace light
{
    std::unique_ptr<IAssetDumper> CreateDumperT6(const AssetPool<T6::AssetLightDef::Type>& pool);
} // namespace light
