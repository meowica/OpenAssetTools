#include "ImageLoaderEmbeddedT6.h"

#include "Game/T6/CommonT6.h"
#include "Image/DdsLoader.h"
#include "Image/ImageCommon.h"
#include "Image/IwiTypes.h"
#include "Utils/Logging/Log.h"

#include <cstring>

using namespace T6;
using namespace image;

namespace
{
    class ImageLoaderT6 final : public AssetCreator<AssetImage>
    {
    public:
        ImageLoaderT6(MemoryManager& memory, ISearchPath& searchPath)
            : m_memory(memory),
              m_search_path(searchPath)
        {
        }

        AssetCreationResult CreateAsset(const std::string& assetName, AssetCreationContext& context) override
        {
            // Do not load any GfxImages from raw for now that are not loaded
            // TODO: Load iwis and add streaming info to asset
            if (assetName.empty() || assetName[0] != '*')
                return AssetCreationResult::NoAction();

            const auto file = m_search_path.Open(image::GetFileNameForAsset(assetName, ".dds"));
            if (!file.IsOpen())
                return AssetCreationResult::NoAction();

            const auto texture = image::LoadDds(*file.m_stream);
            if (!texture)
            {
                con::error("Failed to load dds file for image asset \"{}\"", assetName);
                return AssetCreationResult::Failure();
            }

            auto* image = m_memory.Alloc<GfxImage>();
            image->name = m_memory.Dup(assetName.c_str());
            image->picmip.platform[0] = 0;
            image->picmip.platform[1] = 0;
            image->noPicmip = !texture->HasMipMaps();
            image->semantic = TS_FUNCTION;
            image->track = 0;
            image->cardMemory.platform[0] = 0;
            image->cardMemory.platform[1] = 0;
            image->width = static_cast<uint16_t>(texture->GetWidth());
            image->height = static_cast<uint16_t>(texture->GetHeight());
            image->depth = static_cast<uint16_t>(texture->GetDepth());
            image->category = IMG_CATEGORY_AUTO_GENERATED;
            image->delayLoadPixels = false;

            switch (texture->GetTextureType())
            {
            case TextureType::T_2D:
                image->mapType = MAPTYPE_2D;
                break;

            case TextureType::T_3D:
                image->mapType = MAPTYPE_3D;
                break;

            case TextureType::T_CUBE:
                image->mapType = MAPTYPE_CUBE;
                break;

            default:
                image->mapType = MAPTYPE_NONE;
                break;
            }

            const auto mipCount = texture->HasMipMaps() ? texture->GetMipMapCount() : 1;
            const auto faceCount = texture->GetFaceCount();

            auto dataSize = 0uz;
            for (auto mipLevel = 0; mipLevel < mipCount; mipLevel++)
                dataSize += texture->GetSizeOfMipLevel(mipLevel) * faceCount;

            auto* loadDef = static_cast<GfxImageLoadDef*>(m_memory.AllocRaw(offsetof(GfxImageLoadDef, data) + dataSize));
            image->texture.loadDef = loadDef;
            loadDef->levelCount = static_cast<char>(mipCount);
            loadDef->flags = 0;
            if (!texture->HasMipMaps())
                loadDef->flags |= iwi27::IMG_FLAG_NOMIPMAPS;
            if (texture->GetTextureType() == TextureType::T_CUBE)
                loadDef->flags |= iwi27::IMG_FLAG_CUBEMAP;
            if (texture->GetTextureType() == TextureType::T_3D)
                loadDef->flags |= iwi27::IMG_FLAG_VOLMAP;
        

            loadDef->format = static_cast<int>(texture->GetFormat()->GetDxgiFormat());
            loadDef->resourceSize = static_cast<unsigned>(dataSize);

            char* currentDataBuffer = loadDef->data;
            for (auto mipLevel = 0; mipLevel < mipCount; mipLevel++)
            {
                const auto mipSize = texture->GetSizeOfMipLevel(mipLevel);

                for (auto face = 0; face < faceCount; face++)
                {
                    memcpy(currentDataBuffer, texture->GetBufferForMipLevel(mipLevel, face), mipSize);
                    currentDataBuffer += mipSize;
                }
            }

            return AssetCreationResult::Success(context.AddAsset<AssetImage>(assetName, image));
        }

        MemoryManager& m_memory;
        ISearchPath& m_search_path;
    };
} // namespace

namespace image
{
    std::unique_ptr<AssetCreator<AssetImage>> CreateLoaderEmbeddedT6(MemoryManager& memory, ISearchPath& searchPath)
    {
        return std::make_unique<ImageLoaderT6>(memory, searchPath);
    }
} // namespace image
