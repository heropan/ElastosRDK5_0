
#ifndef __ELASTOS_DROID_JAVAPROXY_CIAUDIOFOCUSDISPATCHER_H__
#define __ELASTOS_DROID_JAVAPROXY_CIAUDIOFOCUSDISPATCHER_H__

#include "_Elastos_Droid_JavaProxy_CIAudioFocusDispatcher.h"
#include <elastos/core/Object.h>
#include <jni.h>

using Elastos::Droid::Os::IBinder;
namespace Elastos {
namespace Droid {
namespace JavaProxy {

CarClass(CIAudioFocusDispatcher)
    , public Object
    , public IBinder
{
public:
    ~CIAudioFocusDispatcher();

    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CARAPI constructor(
        /* [in] */ Handle32 jVM,
        /* [in] */ Handle32 jInstance);

    CARAPI DispatchAudioFocusChange(
        /* [in] */ Int32 focusChange,
        /* [in] */ const String& clientId);

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

#endif // __ELASTOS_DROID_JAVAPROXY_CIAUDIOFOCUSDISPATCHER_H__
