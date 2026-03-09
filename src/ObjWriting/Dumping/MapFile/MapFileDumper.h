#pragma once

#include "Dumping/AbstractTextDumper.h"
#include "Dumping/AssetDumpingContext.h"
#include "Dumping/IZoneAssetDumperState.h"

#include <ostream>
#include <unordered_map>

class MapFileDumper : AbstractTextDumper
{
public:
    union Vec3
    {
        struct
        {
            float m_x;
            float m_y;
            float m_z;
        };

        float v[3];

        Vec3(float x, float y, float z);
        explicit Vec3(float v[3]);
    };

private:
    struct
    {
        bool m_in_entity : 1;
        bool m_in_brush : 1;
    } m_flags;

    size_t m_entity_index;
    size_t m_brush_index;

public:
    explicit MapFileDumper(std::ostream& stream);

    void Init();
    void InitWorldSpawn();

    void BeginEntity();
    void EndEntity();

    void BeginBrush();
    void EndBrush();

    void WriteKeyValue(const std::string& key, const std::string& value) const;
};

class MapFileDumperState final : public IZoneAssetDumperState
{
public:
    MapFileDumper* GetOrCreateDumper(AssetDumpingContext& context, const std::string& mapName);

private:
    struct MapFileEntry
    {
        std::unique_ptr<std::ostream> m_file;
        std::unique_ptr<MapFileDumper> m_dumper;
    };

    std::unordered_map<std::string, MapFileEntry> m_dumpers;
};
