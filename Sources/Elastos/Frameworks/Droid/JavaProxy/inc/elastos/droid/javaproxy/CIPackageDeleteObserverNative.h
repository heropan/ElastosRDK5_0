
#ifndef __ELASTOS_DROID_JAVAPROXY_CIPACKAGEDELETEOBSERVERNATIVE_H__
#define __ELASTOS_DROID_JAVAPROXY_CIPACKAGEDELETEOBSERVERNATIVE_H__

#include "_Elastos_Droid_JavaProxy_CIPackageDeleteObserverNative.h"
#include <elastos/core/Object.h>
#include <jni.h>

using Elastos::Droid::Os::IBinder;
namespace Elastos {
namespace Droid {
namespace JavaProxy {

CarClass(CIPackageDeleteObserverNative)
    , public Object
    , public IBinder
{
public:
    ~CIPackageDeleteObserverNative();

    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CARAPI constructor(
        /* [in] */ Handle32 jVM,
        /* [in] */ Handle32 jInstance);

    CARAPI PackageDeleted(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 returnCode);

    CARAPI ToString(
        /* [out] */ String* str);

private:
    static const String TAG;

    JavaVM* mJVM;
    jobject mJInstance;
};

}
}
}

#endif // __ELASTOS_DROID_JAVAPROXY_CIPACKAGEDELETEOBSERVERNATIVE_H__
