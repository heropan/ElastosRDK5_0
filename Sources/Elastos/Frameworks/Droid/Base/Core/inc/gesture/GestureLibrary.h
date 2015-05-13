#ifndef __GESTURELIBRARY_H__
#define __GESTURELIBRARY_H__

#include "gesture/CGestureStore.h"
#include "gesture/Learner.h"

namespace Elastos {
namespace Droid {
namespace Gesture {

class GestureLibrary
{
public:
    virtual CARAPI Save(
        /* [out] */ Boolean *isSaved) = 0;

    virtual CARAPI Load(
        /* [out] */ Boolean *isLoaded) = 0;

    virtual CARAPI IsReadOnly(
        /* [out] */ Boolean *isReadOnly);

    /** @hide */
    virtual CARAPI_(AutoPtr<Learner>) GetLearner();

    virtual CARAPI SetOrientationStyle(
        /* [in] */ Int32 style);

    virtual CARAPI GetOrientationStyle(
        /* [out] */ Int32 *style);

    virtual CARAPI SetSequenceType(
        /* [in] */ Int32 type);

    virtual CARAPI GetSequenceType(
        /* [out] */ Int32 *type);

    virtual CARAPI GetGestureEntries(
        /* [out] */ IObjectContainer **entries);

    virtual CARAPI Recognize(
        /* [in] */ IGesture *gesture,
        /* [out] */ IArrayList** list);

    virtual CARAPI AddGesture(
        /* [in] */  const String& entryName,
        /* [in] */  IGesture *gesture);

    virtual CARAPI RemoveGesture(
        /* [in] */ const String& entryName,
        /* [in] */ IGesture *gesture);

    virtual CARAPI RemoveEntry(
        /* [in] */ const String& entryName);

    virtual CARAPI GetGestures(
        /* [in] */ const String& entryName,
        /* [out] */ IObjectContainer** gestures);

protected:
    GestureLibrary();

public:
    AutoPtr<IGestureStore> mStore;

};

}//namespace Gesture
}//namespace Droid
}//namespace Elastos
#endif //__GESTURELIBRARY_H__
