
#ifndef __ELASTOS_DROID_JAVAPROXY_CIWALLPAPERMANAGERCALLBACKNATIVE_H__
#define __ELASTOS_DROID_JAVAPROXY_CIWALLPAPERMANAGERCALLBACKNATIVE_H__

#include "_Elastos_Droid_JavaProxy_CIWallpaperManagerCallbackNative.h"
#include <elastos/core/Object.h>
#include <jni.h>

using Elastos::Droid::Os::IBinder;
namespace Elastos {
namespace Droid {
namespace JavaProxy {

CarClass(CIWallpaperManagerCallbackNative)
    , public Object
    , public IBinder
{
public:
    ~CIWallpaperManagerCallbackNative();

    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CARAPI constructor(
        /* [in] */ Handle32 jVM,
        /* [in] */ Handle32 jInstance);

    CARAPI OnWallpaperChanged();

private:
    static const String TAG;

    JavaVM* mJVM;
    jobject mJInstance;
};

}
}
}

#endif // __ELASTOS_DROID_JAVAPROXY_CIWALLPAPERMANAGERCALLBACKNATIVE_H__
