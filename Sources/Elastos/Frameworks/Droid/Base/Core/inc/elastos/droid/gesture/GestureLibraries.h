#ifndef __ELASTOS_DROID_GESTURE_GESTURELIBRARIES_H__
#define __ELASTOS_DROID_GESTURE_GESTURELIBRARIES_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/gesture/GestureLibrary.h"

#include "_Elastos_Droid_Gesture_GestureLibraries.h"

using Elastos::IO::IFile;
using Elastos::IO::CFile;
using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace Gesture {

class GestureLibraries
    : public Object
    , public IGestureLibraries
{
private:
    class FileGestureLibrary
        : public GestureLibraries
    {
    public:
        CAR_INTERFACE_DECL();

        FileGestureLibrary(
            /* [in] */ IFile *path);

        virtual ~FileGestureLibrary();

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

    private:
        const AutoPtr<IFile> mPath;
    };

    class ResourceGestureLibrary
        : public GestureLibraries
    {
    public:
        CAR_INTERFACE_DECL();

        ResourceGestureLibrary(
            /* [in] */ IContext *context,
            /* [in] */ Int32 resourceId);

        virtual ~ResourceGestureLibrary();

    private:
//        private final WeakReference<Context> mContext;
        AutoPtr<IWeakReference> mContext;
        const Int32 mResourceId;

    };

public:
    CAR_OBJECT_DECL();

    GestureLibraries();
    virtual ~GestureLibraries();

    CARAPI FromFile(
        /* [in] */ const String& path,
        /* [out] */ IGestureLibrary **instance);

    CARAPI FromFileEx(
        /* [in] */ IFile *path,
        /* [out] */ IGestureLibrary **instance);

    CARAPI FromPrivateFile(
        /* [in] */ IContext *context,
        /* [in] */ const String& name,
        /* [out] */ IGestureLibrary **instance);

    CARAPI FromRawResource(
        /* [in] */ IContext *context,
        /* [in] */ Int32 resourceId,
        /* [out] */ IGestureLibrary **instance);

};

} // namespace Gesture
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_GESTURE_GESTURELIBRARIES_H__
