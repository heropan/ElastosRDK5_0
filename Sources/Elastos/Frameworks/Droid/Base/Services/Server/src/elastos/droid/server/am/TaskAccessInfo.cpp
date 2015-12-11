#include "elastos/droid/server/am/TaskAccessInfo.h"

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

TaskAccessInfo::SubTask::SubTask()
    : mIndex(0)
{}

TaskAccessInfo::TaskAccessInfo()
    : mRootIndex(0)
{}

CAR_INTERFACE_IMPL(TaskAccessInfo, IActivityManagerTaskThumbnails);

TaskAccessInfo::GetMainThumbnail(
    /* [out] */ IBitmap** thumbnail)
{
    VALIDATE_NOT_NULL(thumbnail);
    return ActivityManagerTaskThumbnails::GetMainThumbnail(thumbnail);
}

TaskAccessInfo::SetMainThumbnail(
    /* [in] */ IBitmap* thumbnail)
{
    return ActivityManagerTaskThumbnails::SetMainThumbnail(thumbnail);
}

TaskAccessInfo::GetNumSubThumbbails(
    /* [out] */ Int32* num)
{
    VALIDATE_NOT_NULL(num);
    return ActivityManagerTaskThumbnails::GetNumSubThumbbails(num);
}

TaskAccessInfo::SetNumSubThumbbails(
    /* [in] */ Int32 num)
{
    return ActivityManagerTaskThumbnails::SetNumSubThumbbails(num);
}

/** @hide */
TaskAccessInfo::GetRetriever(
    /* [out] */ IThumbnailRetriever** retriever)
{
    VALIDATE_NOT_NULL(retriever);
    return ActivityManagerTaskThumbnails::GetRetriever(retriever);
}

TaskAccessInfo::SetRetriever(
    /* [in] */ IThumbnailRetriever* retriever)
{
    return ActivityManagerTaskThumbnails::SetRetriever(retriever);
}

TaskAccessInfo::GetSubThumbnail(
    /* [in] */ Int32 index,
    /* [out] */ IBitmap** thumbnail)
{
    VALIDATE_NOT_NULL(thumbnail);
    return ActivityManagerTaskThumbnails::GetSubThumbnail(index, thumbnail);
}

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos
