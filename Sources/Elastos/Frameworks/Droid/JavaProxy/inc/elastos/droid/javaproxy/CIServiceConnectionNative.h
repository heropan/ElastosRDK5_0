
#ifndef __ELASTOS_DROID_JAVAPROXY_CISERVICECONNECTIONNATIVE_H__
#define __ELASTOS_DROID_JAVAPROXY_CISERVICECONNECTIONNATIVE_H__

#include "_Elastos_Droid_JavaProxy_CIServiceConnectionNative.h"
#include <jni.h>

using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Os::IBinder;

namespace Elastos {
namespace Droid {
namespace JavaProxy {

CarClass(CIServiceConnectionNative)
{
public:
    ~CIServiceConnectionNative();

    CARAPI constructor(
        /* [in] */ Handle32 jVM,
        /* [in] */ Handle32 jInstance);

    CARAPI Connected(
        /* [in] */ IComponentName* name,
        /* [in] */ IBinder* service);

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

#endif // __ELASTOS_DROID_JAVAPROXY_CISERVICECONNECTIONNATIVE_H__
