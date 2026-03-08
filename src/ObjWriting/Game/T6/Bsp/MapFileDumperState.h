#pragma once

#include "Bsp/BSPCommon.h"
#include "Dumping/IZoneAssetDumperState.h"
#include "Dumping/MapFile/MapFileDumper.h"

#include <memory>
#include <ostream>
#include <string>
#include <unordered_map>

class AssetDumpingContext;

class MapFileDumperState final : public IZoneAssetDumperState
{
public:
    MapFileDumper* GetOrCreateDumper(AssetDumpingContext& context, const std::string& mapName);

private:
    struct MapFileEntry
    {
        std::unique_ptr<std::ostream> file;
        std::unique_ptr<MapFileDumper> dumper;
    };

    std::unordered_map<std::string, MapFileEntry> m_dumpers;
};
