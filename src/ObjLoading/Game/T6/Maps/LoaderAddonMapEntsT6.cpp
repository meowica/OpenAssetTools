#include "LoaderMapEntsT6.h"

#include "Game/T6/CommonT6.h"
#include "Game/T6/T6.h"

#include "Utils/Logging/Log.h"

#include <cstring>

using namespace T6;

namespace
{
    class AddonMapEntsLoader final : public AssetCreator<AssetAddonMapEnts>
    {
    public:
        AddonMapEntsLoader(MemoryManager& memory, ISearchPath& searchPath)
            : m_memory(memory),
              m_search_path(searchPath)
        {
        }

        AssetCreationResult CreateAsset(const std::string& assetName, AssetCreationContext& context) override
        {
            const auto file = m_search_path.Open(assetName);
            if (!file.IsOpen())
                return AssetCreationResult::NoAction();

            auto* addonMapEnts = m_memory.Alloc<AddonMapEnts>();

            addonMapEnts->name = m_memory.Dup(assetName.c_str());

            auto* buffer = m_memory.Alloc<char>(static_cast<size_t>(file.m_length + 1));
            file.m_stream->read(buffer, file.m_length);

            if (file.m_stream->gcount() != file.m_length)
                return AssetCreationResult::Failure();

            buffer[file.m_length] = '\0';

            // Skip the iwmap header
            char* fileStart = std::strchr(buffer, '{');
            if (!fileStart)
                return AssetCreationResult::Failure();

            addonMapEnts->entityString = fileStart;
            addonMapEnts->numEntityChars = static_cast<int>(std::strlen(fileStart) + 1);

            return AssetCreationResult::Success(context.AddAsset<AssetAddonMapEnts>(assetName, addonMapEnts));
        }

    private:
        MemoryManager& m_memory;
        ISearchPath& m_search_path;
    };
} // namespace

namespace addon_map_ents
{
    std::unique_ptr<AssetCreator<AssetAddonMapEnts>> CreateLoaderT6(MemoryManager& memory, ISearchPath& searchPath)
    {
        return std::make_unique<AddonMapEntsLoader>(memory, searchPath);
    }
} // namespace addon_map_ents
