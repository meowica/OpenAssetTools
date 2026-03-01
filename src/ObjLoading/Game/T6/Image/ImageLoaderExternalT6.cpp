#include "ImageLoaderExternalT6.h"

#include "Game/T6/CommonT6.h"
#include "Image/ImageLoaderCommon.h"
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
            const auto loadingResult = LoadImageCommon(assetName, m_search_path, IwiVersion::IWI_27, CommonImageLoaderHashType::CRC32);
            const auto earlyReturn = loadingResult.GetResultIfCancelled();
            if (earlyReturn)
                return *earlyReturn;
            
            const auto* texture = loadingResult.m_texture.get();

            auto* image = m_memory.Alloc<GfxImage>();
            image->name = m_memory.Dup(assetName.c_str());
            image->delayLoadPixels = true;

            image->noPicmip = loadingResult.m_meta.m_no_picmip;
            image->width = static_cast<uint16_t>(texture->GetWidth());
            image->height = static_cast<uint16_t>(texture->GetHeight());
            image->depth = static_cast<uint16_t>(texture->GetDepth());

            image->streaming = 1;
            image->streamedParts[0].levelCount = 1;
            image->streamedParts[0].levelSize = static_cast<uint32_t>(loadingResult.m_iwi_size);
            image->streamedParts[0].hash = loadingResult.m_hash.crc32 & 0x1FFFFFFF;
            image->streamedPartCount = 1;

            image->texture.loadDef = m_memory.Alloc<GfxImageLoadDef>();

            return AssetCreationResult::Success(context.AddAsset<AssetImage>(assetName, image));
        }

        MemoryManager& m_memory;
        ISearchPath& m_search_path;
    };
} // namespace

namespace image
{
    std::unique_ptr<AssetCreator<AssetImage>> CreateLoaderExternalT6(MemoryManager& memory, ISearchPath& searchPath)
    {
        return std::make_unique<ImageLoaderT6>(memory, searchPath);
    }
} // namespace image
