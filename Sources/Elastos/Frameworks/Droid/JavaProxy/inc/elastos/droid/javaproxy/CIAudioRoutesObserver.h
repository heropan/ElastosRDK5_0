
#ifndef __ELASTOS_DROID_JAVAPROXY_CIAUDIOROUTESOBSERVER_H__
#define __ELASTOS_DROID_JAVAPROXY_CIAUDIOROUTESOBSERVER_H__

#include "_Elastos_Droid_JavaProxy_CIAudioRoutesObserver.h"
#include <elastos/core/Object.h>
#include <jni.h>

using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Media::IAudioRoutesInfo;

namespace Elastos {
namespace Droid {
namespace JavaProxy {

CarClass(CIAudioRoutesObserver)
    , public Object
    , public IBinder
{
public:
    ~CIAudioRoutesObserver();

    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CARAPI constructor(
        /* [in] */ Handle32 jVM,
        /* [in] */ Handle32 jInstance);

    CARAPI DispatchAudioRoutesChanged(
        /* [in] */ IAudioRoutesInfo* newRoutes);

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

#endif // __ELASTOS_DROID_JAVAPROXY_CIAUDIOROUTESOBSERVER_H__
