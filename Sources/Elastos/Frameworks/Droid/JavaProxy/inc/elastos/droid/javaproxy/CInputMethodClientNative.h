
#ifndef __ELASTOS_DROID_JAVAPROXY_CINPUTMETHODCLIENTNATIVE_H__
#define __ELASTOS_DROID_JAVAPROXY_CINPUTMETHODCLIENTNATIVE_H__

#include "_Elastos_Droid_JavaProxy_CInputMethodClientNative.h"
#include <jni.h>

using Elastos::Droid::Internal::View::IInputBindResult;

namespace Elastos {
namespace Droid {
namespace JavaProxy {

CarClass(CInputMethodClientNative)
{
public:
    ~CInputMethodClientNative();

    CARAPI constructor(
        /* [in] */ Handle32 jVM,
        /* [in] */ Handle32 jInstance);

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI SetUsingInputMethod(
        /* [in] */ Boolean state);

    CARAPI OnBindMethod(
        /* [in] */ IInputBindResult* res);

    CARAPI OnUnbindMethod(
        /* [in] */ Int32 sequence);

    CARAPI SetActive(
        /* [in] */ Boolean active);

private:
    static const String TAG;

    JavaVM* mJVM;
    jobject mJInstance;
};

}
}
}

#endif // __ELASTOS_DROID_JAVAPROXY_CINPUTMETHODCLIENTNATIVE_H__
