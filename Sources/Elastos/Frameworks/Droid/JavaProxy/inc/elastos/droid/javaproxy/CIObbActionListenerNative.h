
#ifndef __ELASTOS_DROID_JAVAPROXY_CIOBBACTIONLISTENERNATIVE_H__
#define __ELASTOS_DROID_JAVAPROXY_CIOBBACTIONLISTENERNATIVE_H__

#include "_Elastos_Droid_JavaProxy_CIObbActionListenerNative.h"
#include <elastos/core/Object.h>
#include <jni.h>

using Elastos::Droid::Os::IBinder;
namespace Elastos {
namespace Droid {
namespace JavaProxy {

CarClass(CIObbActionListenerNative)
    , public Object
    , public IBinder
{
public:
    ~CIObbActionListenerNative();

    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CARAPI constructor(
        /* [in] */ Handle32 jVM,
        /* [in] */ Handle32 jInstance);

    CARAPI OnObbResult(
        /* [in] */ const String& filename,
        /* [in] */ Int32 nonce,
        /* [in] */ Int32 status);

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

#endif // __ELASTOS_DROID_JAVAPROXY_CIOBBACTIONLISTENERNATIVE_H__
