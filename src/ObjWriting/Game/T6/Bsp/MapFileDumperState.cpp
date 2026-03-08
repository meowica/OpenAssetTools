#include "Dumping/AssetDumpingContext.h"
#include "MapFileDumperState.h"

MapFileDumper* MapFileDumperState::GetOrCreateDumper(AssetDumpingContext& context, const std::string& mapName)
{
    const auto it = m_dumpers.find(mapName);
    if (it != m_dumpers.end())
        return it->second.dumper.get();

    MapFileEntry entry;
    entry.file = context.OpenAssetFile(bsp_common::GetFileNameForMap(mapName));
    if (!entry.file)
        return nullptr;

    entry.dumper = std::make_unique<MapFileDumper>(*entry.file);
    entry.dumper->Init();

    auto* dumperPtr = entry.dumper.get();
    m_dumpers.emplace(mapName, std::move(entry));
    return dumperPtr;
}
