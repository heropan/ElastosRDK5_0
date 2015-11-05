#ifndef __ELASTOS_DROID_GESTURE_GESTURELIBRARY_H__
#define __ELASTOS_DROID_GESTURE_GESTURELIBRARY_H__

#include "elastos/droid/gesture/CGestureStore.h"
#include "elastos/droid/gesture/Learner.h"

namespace Elastos {
namespace Droid {
namespace Gesture {

class GestureLibrary
    : public Object
    , public IGestureLibrary
{
public:
    CAR_INTERFACE_DECL();

    GestureLibrary();
    virtual ~GestureLibrary();

    CARAPI Save(
        /* [out] */ Boolean *isSaved) = 0;

    CARAPI Load(
        /* [out] */ Boolean *isLoaded) = 0;

    CARAPI IsReadOnly(
        /* [out] */ Boolean *isReadOnly);

    /** @hide */
    CARAPI_(AutoPtr<Learner>) GetLearner();

    CARAPI SetOrientationStyle(
        /* [in] */ Int32 style);

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
        /* [out] */ IArrayList** list);

    CARAPI AddGesture(
        /* [in] */  const String& entryName,
        /* [in] */  IGesture *gesture);

    CARAPI RemoveGesture(
        /* [in] */ const String& entryName,
        /* [in] */ IGesture *gesture);

    CARAPI RemoveEntry(
        /* [in] */ const String& entryName);

    CARAPI GetGestures(
        /* [in] */ const String& entryName,
        /* [out] */ IObjectContainer** gestures);

private:
    AutoPtr<IGestureStore> mStore;

};

} // namespace Gesture
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_GESTURE_GESTURELIBRARY_H__
