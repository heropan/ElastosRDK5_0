#ifndef __ELASTOS_DROID_GESTURE_CFILEGESTURELIBRARY_H__
#define __ELASTOS_DROID_GESTURE_CFILEGESTURELIBRARY_H__

#include "_Elastos_Droid_Gesture_CFileGestureLibrary.h"
#include "GestureLibraries.h"

namespace Elastos {
namespace Droid {
namespace Gesture {

CarClass(CFileGestureLibrary), public GestureLibraries::FileGestureLibrary
{
public:

    CARAPI constructor(
        /* [in] */ IFile *path);

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
