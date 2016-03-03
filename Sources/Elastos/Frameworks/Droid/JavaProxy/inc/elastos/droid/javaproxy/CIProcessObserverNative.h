
#ifndef __ELASTOS_DROID_JAVAPROXY_CIPROCESSOBSERVERNATIVE_H__
#define __ELASTOS_DROID_JAVAPROXY_CIPROCESSOBSERVERNATIVE_H__

#include "_Elastos_Droid_JavaProxy_CIProcessObserverNative.h"
#include <jni.h>

namespace Elastos {
namespace Droid {
namespace JavaProxy {

CarClass(CIProcessObserverNative)
{
public:
    ~CIProcessObserverNative();

    CARAPI constructor(
        /* [in] */ Handle32 jVM,
        /* [in] */ Handle32 jInstance);

    CARAPI OnForegroundActivitiesChanged(
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [in] */ Boolean foregroundActivities);

    CARAPI OnImportanceChanged(
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 importance);

    CARAPI OnProcessDied(
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid);

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

#endif // __ELASTOS_DROID_JAVAPROXY_CIPROCESSOBSERVERNATIVE_H__
