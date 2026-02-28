#pragma once

#include "Game/T6/T6.h"
#include "Menu/IMenuWriter.h"

#include <memory>
#include <string>

namespace menu
{
    class IWriterT6 : public IWriter
    {
    public:
        virtual void WriteMenu(const T6::menuDef_t& menu) = 0;
    };

    std::unique_ptr<IWriterT6> CreateMenuWriterT6(std::ostream& stream);
} // namespace menu
