#pragma once

#include "Dumping/AbstractAssetDumper.h"
#include "Game/T6/T6.h"

#include <memory>

namespace font
{
    std::unique_ptr<IAssetDumper> CreateDumperT6();
} // namespace font
