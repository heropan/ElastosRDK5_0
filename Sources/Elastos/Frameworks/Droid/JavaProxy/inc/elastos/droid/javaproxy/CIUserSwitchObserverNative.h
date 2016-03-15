
#ifndef __ELASTOS_DROID_JAVAPROXY_CIUSERSWITCHOBSERVERNATIVE_H__
#define __ELASTOS_DROID_JAVAPROXY_CIUSERSWITCHOBSERVERNATIVE_H__

#include "_Elastos_Droid_JavaProxy_CIUserSwitchObserverNative.h"
#include <elastos/core/Object.h>
#include <jni.h>

using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::IRemoteCallback;

namespace Elastos {
namespace Droid {
namespace JavaProxy {

CarClass(CIUserSwitchObserverNative)
    , public Object
    , public IBinder
{
public:
    ~CIUserSwitchObserverNative();

    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CARAPI constructor(
        /* [in] */ Handle32 jVM,
        /* [in] */ Handle32 jInstance);

    CARAPI OnUserSwitching(
        /* [in] */ Int32 newUserId,
        /* [in] */ IRemoteCallback* reply);

    CARAPI OnUserSwitchComplete(
        /* [in] */ Int32 newUserId);

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

#endif // __ELASTOS_DROID_JAVAPROXY_CIUSERSWITCHOBSERVERNATIVE_H__
