#include "LightDefDumperT6.h"

#include "LightDefBinaryDumperT6.h"
#include "LightDefJsonDumperT6.h"

using namespace T6;

// #define DUMP_LIGHT_DEF_AS_BINARY 1

namespace light_def
{
    std::unique_ptr<IAssetDumper> CreateDumperT6()
    {
#ifdef DUMP_LIGHT_AS_BINARY
        return std::make_unique<CsvDumperT6>();
#else
        return std::make_unique<JsonDumperT6>();
#endif
    }
} // namespace light_def
