#pragma once

#include "Dumping/AbstractTextDumper.h"

#include <ostream>

class FxFileDumper : AbstractTextDumper
{
public:
    explicit FxFileDumper(std::ostream& stream);

    void Init() const;
};
