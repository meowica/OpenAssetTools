#include "CommonAsset.h"

#include "T6/CommonAssetT6.h"

#include <cassert>

ICommonAssetTypeMapper* ICommonAssetTypeMapper::GetCommonAssetMapperByGame(GameId)
{
    static T6::CommonAssetTypeMapper mapper{};
    return &mapper;
}
