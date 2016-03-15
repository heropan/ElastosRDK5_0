
#ifndef __ELASTOS_DROID_JAVAPROXY_CIINPUTFILTERNATIVE_H__
#define __ELASTOS_DROID_JAVAPROXY_CIINPUTFILTERNATIVE_H__

#include "_Elastos_Droid_JavaProxy_CIInputFilterNative.h"
#include <elastos/core/Object.h>
#include <jni.h>

using Elastos::Droid::Os::IBinder;
using Elastos::Droid::View::IInputEvent;
using Elastos::Droid::View::IInputFilterHost;

namespace Elastos {
namespace Droid {
namespace JavaProxy {

CarClass(CIInputFilterNative)
    , public Object
    , public IBinder
{
public:
    ~CIInputFilterNative();

    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CARAPI constructor(
        /* [in] */ Handle32 jVM,
        /* [in] */ Handle32 jInstance);

    CARAPI Install(
        /* [in] */ IInputFilterHost* host);

    CARAPI Uninstall();

    CARAPI FilterInputEvent(
        /* [in] */ IInputEvent* event,
        /* [in] */ Int32 policyFlags);

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

#endif // __ELASTOS_DROID_JAVAPROXY_CIINPUTFILTERNATIVE_H__
