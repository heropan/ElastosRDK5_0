
#ifndef __ELASTOS_DROID_APP_CACTIVITYMANAGERTASKTHUMBNAILS_H__
#define __ELASTOS_DROID_APP_CACTIVITYMANAGERTASKTHUMBNAILS_H__

#include "_CActivityManagerTaskThumbnails.h"
#include "ActivityManagerTaskThumbnails.h"

using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::App::ActivityManagerTaskThumbnails;

namespace Elastos {
namespace Droid {
namespace App {

/** @hide */
CarClass(CActivityManagerTaskThumbnails)
    , public ActivityManagerTaskThumbnails
{
public:
    CARAPI constructor();

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
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_APP_CACTIVITYMANAGERTASKTHUMBNAILS_H__
