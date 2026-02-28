#pragma once

#include "Json/JsonCommon.h"
#include "Json/JsonExtension.h"
#include <memory>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <vector>

namespace T6
{
    class JsonLightDef
    {
    public:
        std::string name;
        std::string attenuationTexture;
        char samplerState = 0;
        int lmapLookupStart = 0;
    };

    NLOHMANN_DEFINE_TYPE_EXTENSION(JsonLightDef, name, attenuationTexture, samplerState, lmapLookupStart);
} // namespace T6
