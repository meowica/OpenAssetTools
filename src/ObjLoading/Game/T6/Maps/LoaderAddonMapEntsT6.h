#pragma once

#include "Asset/IAssetCreator.h"
#include "Game/T6/T6.h"
#include "SearchPath/ISearchPath.h"
#include "Utils/MemoryManager.h"

#include <memory>

namespace addon_map_ents
{
    std::unique_ptr<AssetCreator<T6::AssetAddonMapEnts>> CreateLoaderT6(MemoryManager& memory, ISearchPath& searchPath);
} // namespace addon_map_ents
