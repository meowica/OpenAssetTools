#include "MapFileDumper.h"

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

void MapFileDumper::Init() const
{
    m_stream << "iwmap 4\n";
    m_stream << "\"000_Global\" flags  active\n";
    m_stream << "\"The Map\" flags\n";
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
