#ifndef __CRESOURCEGESTURELIBRARY_H__
#define __CRESOURCEGESTURELIBRARY_H__

#include "_CResourceGestureLibrary.h"
#include "GestureLibraries.h"

namespace Elastos {
namespace Droid {
namespace Gesture {

CarClass(CResourceGestureLibrary), public GestureLibraries::ResourceGestureLibrary
{
public:

    CARAPI constructor(
        /* [in] */ IContext *context,
        /* [in] */ Int32 resourceId);

    CARAPI Save(
        /* [out] */ Boolean *isSaved);

    CARAPI Load(
        /* [out] */ Boolean *isLoaded);

    CARAPI IsReadOnly(
        /* [out] */ Boolean *isReadOnly);

    CARAPI GetLearner(
        /* [out] */ ILearner **learner);

    CARAPI SetOrientationStyle(
        /* [out] */ Int32 style);

    CARAPI GetOrientationStyle(
        /* [out] */ Int32 *style);

    CARAPI SetSequenceType(
        /* [in] */ Int32 type);

    CARAPI GetSequenceType(
        /* [out] */ Int32 *type);

    CARAPI GetGestureEntries(
        /* [out] */ IObjectContainer **entries);

    CARAPI Recognize(
        /* [in] */ IGesture *gesture,
        /* [out] */ IObjectContainer **predictions);

    CARAPI AddGesture(
        /* [in] */ const String& entryName,
        /* [in] */ IGesture *gesture);

    CARAPI RemoveGesture(
        /* [in] */ const String& entryName,
        /* [in] */ IGesture *gesture);

    CARAPI RemoveEntry(
        /* [in] */ const String& entryName);

    CARAPI GetGestures(
        /* [in] */ const String& entryName,
        /* [out] */ IObjectContainer **gestures);
};

}
}
}

#endif
