
#ifndef __ELASTOS_DROID_SERVER_AM_TASKACCESSINFO_H__
#define __ELASTOS_DROID_SERVER_AM_TASKACCESSINFO_H__

#include "elastos/droid/ext/frameworkext.h"
#include "Elastos.Droid.Server_server.h"
#include <elastos/utility/etl/List.h>
#include "elastos/droid/app/ActivityManagerTaskThumbnails.h"
#include "am/ThumbnailHolder.h"
#include "am/ActivityRecord.h"

using Elastos::Droid::App::IThumbnailRetriever;
using Elastos::Droid::App::ActivityManagerTaskThumbnails;
using Elastos::Droid::App::IActivityManagerTaskThumbnails;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

class TaskAccessInfo
    : public ActivityManagerTaskThumbnails
    , public IActivityManagerTaskThumbnails
    , public ElRefBase
{
public:
    class SubTask : public ElRefBase
    {
    public:
        SubTask();

        AutoPtr<ThumbnailHolder> mHolder;
        AutoPtr<ActivityRecord> mActivity;
        Int32 mIndex;
    };

    TaskAccessInfo();

    CAR_INTERFACE_DECL()

    GetMainThumbnail(
        /* [out] */ IBitmap** thumbnail);

    SetMainThumbnail(
        /* [in] */ IBitmap* thumbnail);

    GetNumSubThumbbails(
        /* [out] */ Int32* num);

    SetNumSubThumbbails(
        /* [in] */ Int32 num);

    /** @hide */
    GetRetriever(
        /* [out] */ IThumbnailRetriever** retriever);

    SetRetriever(
        /* [in] */ IThumbnailRetriever* retriever);

    GetSubThumbnail(
        /* [in] */ Int32 index,
        /* [out] */ IBitmap** thumbnail);

    AutoPtr<ActivityRecord> mRoot;
    Int32 mRootIndex;

    List<AutoPtr<SubTask> > mSubtasks;

};

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SERVER_AM_TASKACCESSINFO_H__
