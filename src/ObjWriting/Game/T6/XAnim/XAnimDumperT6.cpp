#include "XAnimDumperT6.h"

#include "XAnim/XAnimCommon.h"
#include "Json/JsonCommon.h"

#include <cassert>
#include <format>

using namespace T6;
using namespace nlohmann;

namespace
{
    class JsonDumper
    {
    public:
        JsonDumper(AssetDumpingContext& context, std::ostream& stream)
            : m_stream(stream)
        {
        }

        void Dump(const XAnimParts* xanim) const
        {
            nlohmann::json jRoot;

            jRoot["$schema"] = "http://openassettools.dev/schema/xanim.v1.json";
            jRoot["_type"] = "xanim";
            jRoot["_version"] = 1;
            jRoot["_game"] = "t6";

            jRoot["numframes"] = xanim->numframes;
            jRoot["framerate"] = xanim->framerate;
            jRoot["loop"] = xanim->bLoop;
            jRoot["delta"] = xanim->bDelta;
            jRoot["notifyCount"] = xanim->notifyCount;

            if (xanim->notify && xanim->notifyCount > 0)
            {
                auto jNotifies = nlohmann::json::array();
                for (auto i = 0; i < xanim->notifyCount; i++)
                {
                    nlohmann::json jNotify;
                    jNotify["name"] = xanim->notify[i].name;
                    jNotify["time"] = xanim->notify[i].time;
                    jNotifies.emplace_back(std::move(jNotify));
                }
                jRoot["notifies"] = std::move(jNotifies);
            }

            m_stream << std::setw(4) << jRoot << "\n";
        }

    private:
        std::ostream& m_stream;
    };

    void DumpXAnimJson(AssetDumpingContext& context, const XAssetInfo<XAnimParts>& asset)
    {
        const auto assetFile = context.OpenAssetFile(xanim::GetJsonFileNameForAssetName(asset.m_name));
        if (!assetFile)
            return;

        const JsonDumper dumper(context, *assetFile);
        dumper.Dump(asset.Asset());
    }
} // namespace

namespace xanim
{
    void DumperT6::DumpAsset(AssetDumpingContext& context, const XAssetInfo<AssetXAnim::Type>& asset)
    {
        DumpXAnimJson(context, asset);
    }
}
