#include "JsonLoaderLightDefT6.h"

#include "LightDef/LightDefCommon.h"
#include "Game/T6/CommonT6.h"
#include "Game/T6/LightDef/JsonLightDefT6.h"
#include "Game/T6/T6.h"
#include "Utils/Logging/Log.h"

#include <algorithm>
#include <format>
#include <iostream>
#include <nlohmann/json.hpp>

using namespace nlohmann;
using namespace T6;

namespace
{
    class JsonLightDefLoader final : public AssetCreator<AssetLightDef>
    {
    public:
        JsonLightDefLoader(MemoryManager& memory, ISearchPath& searchPath)
            : m_memory(memory),
              m_search_path(searchPath)
        {
        }

        AssetCreationResult CreateAsset(const std::string& assetName, AssetCreationContext& context) override
        {
            const auto file = m_search_path.Open(light_def::GetJsonFileNameForAsset(assetName));
            if (!file.IsOpen())
                return AssetCreationResult::NoAction();

            auto* lightDef = m_memory.Alloc<GfxLightDef>();
            lightDef->name = m_memory.Dup(assetName.c_str());

            AssetRegistration<AssetLightDef> registration(assetName, lightDef);

            try
            {
                const auto jRoot = json::parse(*file.m_stream);
                std::string type;
                unsigned version;

                jRoot.at("_type").get_to(type);
                jRoot.at("_version").get_to(version);

                if (type != "light-def" || version != 1u)
                {
                    con::error("Tried to load light-def \"{}\" but did not find expected type light-def of version 1", assetName);
                    return AssetCreationResult::Failure();
                }

                const auto jLight = jRoot.get<JsonLightDef>();
                if (CreateLightDefFromJson(jLight, *lightDef, registration, context))
                    return AssetCreationResult::Success(context.AddAsset(std::move(registration)));
            }
            catch (const json::exception& e)
            {
                con::error("Failed to parse json of light-def: {}", e.what());
            }

            return AssetCreationResult::Failure();
        }

    private:
        bool CreateLightDefFromJson(const JsonLightDef& jLightDef,
                                    GfxLightDef& lightDef,
                                    AssetRegistration<AssetLightDef>& registration,
                                    AssetCreationContext& context) const
        {
            // Example: Load attenuation texture
            if (!jLightDef.attenuationTexture.empty())
            {
                auto* attenuationAsset = context.LoadDependency<AssetImage>(jLightDef.attenuationTexture);
                if (!attenuationAsset)
                {
                    con::error("Failed to load attenuation texture \"{}\" for light-def \"{}\"", jLightDef.attenuationTexture, lightDef.name);
                    return false;
                }

                registration.AddDependency(attenuationAsset);
                lightDef.attenuation.image = attenuationAsset->Asset();
                lightDef.attenuation.samplerState = jLightDef.samplerState;
            }

            // Load other properties
            lightDef.lmapLookupStart = jLightDef.lmapLookupStart;

            return true;
        }

        MemoryManager& m_memory;
        ISearchPath& m_search_path;
    };
} // namespace

namespace light_def
{
    std::unique_ptr<AssetCreator<AssetLightDef>> CreateJsonLoaderT6(MemoryManager& memory, ISearchPath& searchPath)
    {
        return std::make_unique<JsonLightDefLoader>(memory, searchPath);
    }
} // namespace light_def
