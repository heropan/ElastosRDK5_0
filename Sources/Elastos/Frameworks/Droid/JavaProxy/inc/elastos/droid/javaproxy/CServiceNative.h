
#ifndef __ELASTOS_DROID_JAVAPROXY_CSERVICENATIVE_H__
#define __ELASTOS_DROID_JAVAPROXY_CSERVICENATIVE_H__

#include "_Elastos_Droid_JavaProxy_CServiceNative.h"
#include <jni.h>
#include <binder/Binder.h>

namespace Elastos {
namespace Droid {
namespace JavaProxy {

CarClass(CServiceNative)
{
public:
    ~CServiceNative();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Handle32 jVM,
        /* [in] */ Handle32 jInstance);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI ToString(
        /* [out] */ String* str);

public:
    CARAPI GetRemoteInstance(JNIEnv* env, Handle32* obj);

private:
    static const String TAG;

    JavaVM* mJVM;
    jobject mJInstance;
    android::sp<android::IBinder> mibinder;
};

} // JavaProxy
} // Droid
} // Elastos

#endif // __ELASTOS_DROID_JAVAPROXY_CSERVICENATIVE_H__
