#include "FxFileDumper.h"

FxFileDumper::FxFileDumper(std::ostream& stream)
    : AbstractTextDumper(stream)
{
}

void FxFileDumper::Init() const
{
    m_stream << "iwfx 3\n\n";
    m_stream << "{\n\n";
    m_stream << "}\n\n";
}
