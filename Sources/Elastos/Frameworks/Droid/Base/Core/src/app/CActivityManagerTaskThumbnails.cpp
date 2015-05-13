
#include "app/CActivityManagerTaskThumbnails.h"
#include <ext/frameworkext.h>

namespace Elastos {
namespace Droid {
namespace App {

ECode CActivityManagerTaskThumbnails::constructor()
{
    return ActivityManagerTaskThumbnails::Init();
}

ECode CActivityManagerTaskThumbnails::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return ActivityManagerTaskThumbnails::WriteToParcel(dest);
}

ECode CActivityManagerTaskThumbnails::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return ActivityManagerTaskThumbnails::ReadFromParcel(source);
}

ECode CActivityManagerTaskThumbnails::GetMainThumbnail(
    /* [out] */ IBitmap** thumbnail)
{
    return ActivityManagerTaskThumbnails::GetMainThumbnail(thumbnail);
}

ECode CActivityManagerTaskThumbnails::SetMainThumbnail(
    /* [in] */ IBitmap* thumbnail)
{
    return ActivityManagerTaskThumbnails::SetMainThumbnail(thumbnail);
}

ECode CActivityManagerTaskThumbnails::GetNumSubThumbbails(
    /* [out] */ Int32* num)
{
    return ActivityManagerTaskThumbnails::GetNumSubThumbbails(num);
}

ECode CActivityManagerTaskThumbnails::SetNumSubThumbbails(
    /* [in] */ Int32 num)
{
    return ActivityManagerTaskThumbnails::SetNumSubThumbbails(num);
}

/** @hide */
ECode CActivityManagerTaskThumbnails::GetRetriever(
    /* [out] */ IThumbnailRetriever** retriever)
{
    return ActivityManagerTaskThumbnails::GetRetriever(retriever);
}

ECode CActivityManagerTaskThumbnails::SetRetriever(
    /* [in] */ IThumbnailRetriever* retriever)
{
    return ActivityManagerTaskThumbnails::SetRetriever(retriever);
}

ECode CActivityManagerTaskThumbnails::GetSubThumbnail(
    /* [in] */ Int32 index,
    /* [out] */ IBitmap** thumbnail)
{
    return ActivityManagerTaskThumbnails::GetSubThumbnail(index, thumbnail);
}

} // namespace App
} // namespace Droid
} // namespace Elastos

