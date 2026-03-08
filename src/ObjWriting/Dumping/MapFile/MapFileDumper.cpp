#include "MapFileDumper.h"

#include "Dumping/AssetDumpingContext.h"
#include "Bsp/BSPCommon.h"

#include <cassert>
#include <iomanip>

MapFileDumper::Vec3::Vec3(const float x,
                          const float y,
                          const float z)
    : v{}
{
    m_x = x;
    m_y = y;
    m_z = z;
}

MapFileDumper::Vec3::Vec3(float v[3])
    : v{v[0], v[1], v[2]}
{
}

MapFileDumper::MapFileDumper(std::ostream& stream)
    : AbstractTextDumper(stream),
      m_flags{},
      m_entity_index(0u),
      m_brush_index(0u)
{
}

void MapFileDumper::Init()
{
    m_stream << "iwmap 4\n";
    m_stream << "\"000_Global\" flags  active\n";
    m_stream << "\"The Map\" flags\n";

    m_stream << "// entity 0\n";
    m_stream << "{\n";
    m_stream << "\"classname\" \"worldspawn\"\n";
    m_stream << "}\n";

    m_entity_index = 1; // because of worldspawn
}

void MapFileDumper::BeginEntity()
{
    assert(!m_flags.m_in_entity);
    if (m_flags.m_in_entity)
        return;

    Indent();
    m_stream << "// entity " << m_entity_index << "\n";

    Indent();
    m_stream << "{\n";

    IncIndent();
    m_entity_index++;
    m_brush_index = 0;
    m_flags.m_in_entity = true;
}

void MapFileDumper::EndEntity()
{
    assert(m_flags.m_in_entity);
    if (!m_flags.m_in_entity)
        return;

    DecIndent();
    Indent();
    m_stream << "}\n";
    m_flags.m_in_entity = false;
}

void MapFileDumper::BeginBrush()
{
    assert(m_flags.m_in_entity && !m_flags.m_in_brush);
    if (m_flags.m_in_brush)
        return;

    Indent();
    m_stream << "// brush " << m_brush_index << "\n";

    Indent();
    m_stream << "{\n";

    IncIndent();
    m_brush_index++;
    m_flags.m_in_brush = true;
}

void MapFileDumper::EndBrush()
{
    assert(m_flags.m_in_entity && m_flags.m_in_brush);
    if (!m_flags.m_in_brush)
        return;

    DecIndent();
    Indent();
    m_stream << "}\n";
    m_flags.m_in_brush = false;
}

void MapFileDumper::WriteKeyValue(const std::string& key, const std::string& value) const
{
    assert(m_flags.m_in_brush || m_flags.m_in_entity);
    m_stream << "\"" << key << "\" \"" << value << "\"\n";
}

MapFileDumper* MapFileDumperState::GetOrCreateDumper(AssetDumpingContext& context, const std::string& mapName)
{
    const auto it = m_dumpers.find(mapName);
    if (it != m_dumpers.end())
        return it->second.m_dumper.get();

    MapFileEntry entry;
    entry.m_file = context.OpenAssetFile(bsp_common::GetFileNameForMap(mapName));
    if (!entry.m_file)
        return nullptr;

    entry.m_dumper = std::make_unique<MapFileDumper>(*entry.m_file);
    entry.m_dumper->Init();

    auto* dumperPtr = entry.m_dumper.get();
    m_dumpers.emplace(mapName, std::move(entry));

    return dumperPtr;
}
