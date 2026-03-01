#include "ImageConverter.h"

#include "Game/IGame.h"
#include "Image/DdsLoader.h"
#include "Image/DdsWriter.h"
#include "Image/IwiLoader.h"
#include "Image/IwiWriter27.h"
#include "Image/Texture.h"
#include "ImageConverterArgs.h"
#include "Utils/Logging/Log.h"
#include "Utils/StringUtils.h"

#include <assert.h>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <optional>

namespace fs = std::filesystem;
using namespace image;

namespace
{
    constexpr auto EXTENSION_IWI = ".iwi";
    constexpr auto EXTENSION_DDS = ".dds";

    class ImageConverterImpl final : public ImageConverter
    {
    public:
        ImageConverterImpl() = default;

        bool Start(const int argc, const char** argv) override
        {
            con::init();

            auto shouldContinue = true;
            if (!m_args.ParseArgs(argc, argv, shouldContinue))
                return false;

            if (!shouldContinue)
                return true;

            for (const auto& file : m_args.m_files_to_convert)
                Convert(file);

            return true;
        }

    private:
        void Convert(const std::string& file)
        {
            const fs::path filePath(file);
            auto extension = filePath.extension().string();
            utils::MakeStringLowerCase(extension);

            if (extension == EXTENSION_IWI)
                ConvertIwi(filePath);
            else if (extension == EXTENSION_DDS)
                ConvertDds(filePath);
            else
                con::error("Unsupported extension {}", extension);
        }

        bool ConvertIwi(const fs::path& iwiPath)
        {
            std::ifstream file(iwiPath, std::ios::in | std::ios::binary);
            if (!file.is_open())
            {
                con::error("Failed to open input file {}", iwiPath.string());
                return false;
            }

            const auto loadResult = image::LoadIwi(file);
            if (!loadResult)
                return false;

            auto outPath = iwiPath;
            outPath.replace_extension(".dds");

            std::ofstream outFile(outPath, std::ios::out | std::ios::binary);
            if (!outFile.is_open())
            {
                con::error("Failed to open output file {}", outPath.string());
                return false;
            }

            m_dds_writer.DumpImage(outFile, loadResult->m_texture.get());
            return true;
        }

        bool ConvertDds(const fs::path& ddsPath)
        {
            std::ifstream file(ddsPath, std::ios::in | std::ios::binary);
            if (!file.is_open())
            {
                con::error("Failed to open input file {}", ddsPath.string());
                return false;
            }

            const auto texture = image::LoadDds(file);
            if (!texture)
                return false;

            if (!EnsureIwiWriterIsPresent())
                return false;

            auto outPath = ddsPath;
            outPath.replace_extension(".iwi");

            std::ofstream outFile(outPath, std::ios::out | std::ios::binary);
            if (!outFile.is_open())
            {
                con::error("Failed to open output file {}", outPath.string());
                return false;
            }

            m_iwi_writer->DumpImage(outFile, texture.get());
            return true;
        }

        bool EnsureIwiWriterIsPresent()
        {
            if (!m_iwi_writer)
                m_iwi_writer = std::make_unique<image::iwi27::IwiWriter>();

            return true;
        }

        ImageConverterArgs m_args;
        DdsWriter m_dds_writer;
        std::unique_ptr<ImageWriter> m_iwi_writer;
    };
} // namespace

std::unique_ptr<ImageConverter> ImageConverter::Create()
{
    return std::make_unique<ImageConverterImpl>();
}
