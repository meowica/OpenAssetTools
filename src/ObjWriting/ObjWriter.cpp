#include "ObjWriter.h"

#include "Game/T6/ObjWriterT6.h"
#include "ObjWriting.h"

#include <cassert>

bool IObjWriter::DumpZone(AssetDumpingContext& context)
{
    m_asset_dumpers.clear();
    RegisterAssetDumpers(context);

    if (context.ShouldTrackProgress())
    {
        size_t totalProgress = 0uz;
        for (const auto& dumper : m_asset_dumpers)
            totalProgress += dumper->GetProgressTotalCount(context);

        context.SetTotalProgress(totalProgress);
    }

    for (const auto& dumper : m_asset_dumpers)
        dumper->Dump(context);

    return true;
}

void IObjWriter::RegisterAssetDumper(std::unique_ptr<IAssetDumper> dumper)
{
    const auto maybeHandlingAssetType = dumper->GetHandlingAssetType();
    if (maybeHandlingAssetType.has_value() && !ObjWriting::ShouldHandleAssetType(*maybeHandlingAssetType))
        return;

    m_asset_dumpers.emplace_back(std::move(dumper));
}

IObjWriter* IObjWriter::GetObjWriterForGame(GameId)
{
    static T6::ObjWriter writer{};
    return &writer;
}
