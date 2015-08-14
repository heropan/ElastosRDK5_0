
#ifndef __ELASTOS_DROID_APP_ACTIVITYMANAGERTASKTHUMBNAILS_H__
#define __ELASTOS_DROID_APP_ACTIVITYMANAGERTASKTHUMBNAILS_H__

#include "ext/frameworkext.h"

using Elastos::Droid::Graphics::IBitmap;

namespace Elastos {
namespace Droid {
namespace App {

/** @hide */
class ActivityManagerTaskThumbnails
{
public:
    ActivityManagerTaskThumbnails();

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI GetMainThumbnail(
        /* [out] */ IBitmap** thumbnail);

    CARAPI SetMainThumbnail(
        /* [in] */ IBitmap* thumbnail);

    CARAPI GetNumSubThumbbails(
        /* [out] */ Int32* num);

    CARAPI SetNumSubThumbbails(
        /* [in] */ Int32 num);

    /** @hide */
    CARAPI GetRetriever(
        /* [out] */ IThumbnailRetriever** retriever);

    CARAPI SetRetriever(
        /* [in] */ IThumbnailRetriever* retriever);

    CARAPI GetSubThumbnail(
        /* [in] */ Int32 index,
        /* [out] */ IBitmap** thumbnail);

protected:
    CARAPI Init();

public:
    AutoPtr<IBitmap> mMainThumbnail;

    Int32 mNumSubThumbbails;

    /** @hide */
    AutoPtr<IThumbnailRetriever> mRetriever;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_APP_ACTIVITYMANAGERTASKTHUMBNAILS_H__
