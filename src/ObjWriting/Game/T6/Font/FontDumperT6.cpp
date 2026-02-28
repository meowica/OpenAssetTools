#include "FontDumperT6.h"

#include "FontBinaryDumperT6.h"
#include "FontJsonDumperT6.h"

using namespace T6;

// #define DUMP_FONT_AS_BINARY 1

namespace font
{
    std::unique_ptr<IAssetDumper> CreateDumperT6()
    {
#ifdef DUMP_FONT_AS_BINARY
        return std::make_unique<CsvDumperT6>();
#else
        return std::make_unique<JsonDumperT6>();
#endif
    }
} // namespace font
