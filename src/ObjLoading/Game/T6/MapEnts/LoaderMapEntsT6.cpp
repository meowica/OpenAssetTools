#include "LoaderMapEntsT6.h"

#include "Game/T6/CommonT6.h"
#include "Game/T6/T6.h"

#include <cstring>

using namespace T6;

namespace
{
    class MapEntsLoader final : public AssetCreator<AssetMapEnts>
    {
    public:
        MapEntsLoader(MemoryManager& memory, ISearchPath& searchPath)
            : m_memory(memory),
              m_search_path(searchPath)
        {
        }

        AssetCreationResult CreateAsset(const std::string& assetName, AssetCreationContext& context) override
        {
            const auto file = m_search_path.Open(assetName);
            if (!file.IsOpen())
                return AssetCreationResult::NoAction();

            auto* mapEnts = m_memory.Alloc<MapEnts>();

            // strip the name down to just map.d3dbsp
            std::string cleanName = assetName;
            if (cleanName.ends_with(".mapents"))
                cleanName.erase(cleanName.size() - 8);

            mapEnts->name = m_memory.Dup(cleanName.c_str());
            mapEnts->numEntityChars = static_cast<int>(file.m_length + 1);

            auto* buffer = m_memory.Alloc<char>(static_cast<size_t>(file.m_length + 1));
            file.m_stream->read(buffer, file.m_length);

            if (file.m_stream->gcount() != file.m_length)
                return AssetCreationResult::Failure();

            buffer[file.m_length] = '\0';
            mapEnts->entityString = buffer;

            return AssetCreationResult::Success(context.AddAsset<AssetMapEnts>(cleanName, mapEnts));
        }

    private:
        MemoryManager& m_memory;
        ISearchPath& m_search_path;
    };
} // namespace

namespace map_ents
{
    std::unique_ptr<AssetCreator<AssetMapEnts>> CreateLoaderT6(MemoryManager& memory, ISearchPath& searchPath)
    {
        return std::make_unique<MapEntsLoader>(memory, searchPath);
    }
} // namespace map_ents
