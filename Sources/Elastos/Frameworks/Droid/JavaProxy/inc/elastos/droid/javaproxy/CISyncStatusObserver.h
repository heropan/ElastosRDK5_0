
#ifndef __ELASTOS_DROID_JAVAPROXY_CISYNCSTATUSOBSERVER_H__
#define __ELASTOS_DROID_JAVAPROXY_CISYNCSTATUSOBSERVER_H__

#include "_Elastos_Droid_JavaProxy_CISyncStatusObserver.h"
#include <elastos/core/Object.h>
#include <jni.h>

using Elastos::Droid::Os::IBinder;
namespace Elastos {
namespace Droid {
namespace JavaProxy {

CarClass(CISyncStatusObserver)
    , public Object
    , public IBinder
{
public:
    ~CISyncStatusObserver();

    CARAPI OnStatusChanged(
        /* [in] */ Int32 which);

    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CARAPI constructor(
        /* [in] */ Handle32 jVM,
        /* [in] */ Handle32 jInstance);

private:
    static const String TAG;

    JavaVM* mJVM;
    jobject mJInstance;
};

}
}
}

#endif // __ELASTOS_DROID_JAVAPROXY_CISYNCSTATUSOBSERVER_H__
