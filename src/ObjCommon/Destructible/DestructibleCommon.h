#pragma once

#include <string>

namespace destructible
{
    std::string GetFileNameForDestructibleDef(const std::string& assetName);
    std::string GetFileNameForDestructiblePiece(const std::string& assetName, const int pieceIndex);
}
