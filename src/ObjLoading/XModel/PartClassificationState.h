#pragma once

#include "SearchPath/ISearchPath.h"

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

class PartClassificationState
{
    // TODO: Use MP part classifications when building an MP fastfile
    // How would we even detect such a thing..? search if zone name starts with mp_ or zm_?
    static constexpr auto PART_CLASSIFICATION_FILE = "partclassification.csv";
    // static constexpr auto PART_CLASSIFICATION_FILE_MP = "partclassification_mp.csv";

    static constexpr auto HITLOC_NONE = 0u;

public:
    PartClassificationState();

    bool Load(const char** hitLocNames, size_t hitLocNameCount, ISearchPath& searchPath);

    [[nodiscard]] unsigned GetPartClassificationForBoneName(const std::string& boneName) const;

private:
    bool LoadRow(const char** hitLocStart, const char** hitLocEnd, unsigned rowIndex, std::vector<std::string>& row);

    bool m_loaded;
    std::unordered_map<std::string, unsigned> m_part_classifications;
};
