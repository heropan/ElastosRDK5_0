#ifndef __CGESTURELIBRARIES_H__
#define __CGESTURELIBRARIES_H__

#include "ext/frameworkext.h"
#include "_CGestureLibraries.h"
#include "gesture/GestureLibrary.h"

using Elastos::IO::IFile;
using Elastos::IO::CFile;
using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace Gesture {

CarClass(CGestureLibraries)
{
private:
    class FileGestureLibrary
        : public ElRefBase
        , public GestureLibrary
        , public IGestureLibrary
    {
    public:
        CAR_INTERFACE_DECL()

        FileGestureLibrary(
            /* [in] */ IFile *path);

        //@Override
        CARAPI IsReadOnly(
            /* [out] */ Boolean *isReadOnly);

        CARAPI Save(
            /* [out] */ Boolean *isSaved);

        CARAPI Load(
            /* [out] */ Boolean *isLoaded);

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
            /* [out] */ IArrayList **predictions);

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
            /* [out] */ IObjectContainer **gestures);

    private:
        const AutoPtr<IFile> mPath;
    };

    class ResourceGestureLibrary
        : public ElRefBase
        , public GestureLibrary
        , public IGestureLibrary
    {
    public:
        CAR_INTERFACE_DECL()

        ResourceGestureLibrary(
            /* [in] */ IContext *context,
            /* [in] */ Int32 resourceId);

        //@Override
        CARAPI IsReadOnly(
            /* [out] */ Boolean *isReadOnly);

        CARAPI Save(
            /* [out] */ Boolean *isSaved);

        CARAPI Load(
            /* [out] */ Boolean *isLoaded);

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
            /* [out] */ IArrayList **predictions);

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
            /* [out] */ IObjectContainer **gestures);

    private:
//        private final WeakReference<Context> mContext;
        AutoPtr<IWeakReference> mContext;
        const Int32 mResourceId;

    };
public:
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

    CGestureLibraries();
};

}//namespace Gesture
}//namespace Droid
}//namespace Elastos

#endif //__CGESTURELIBRARIES_H__
