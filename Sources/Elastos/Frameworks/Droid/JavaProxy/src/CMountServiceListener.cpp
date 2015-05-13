
#include "CMountServiceListener.h"
#include <elastos/Logger.h>

using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace JavaProxy {

const String CMountServiceListener::TAG = String("CMountServiceListener");

CMountServiceListener::~CMountServiceListener(){
    JNIEnv* env;
    mJVM->AttachCurrentThread(&env, NULL);
    env->DeleteGlobalRef(mJInstance);
}

ECode CMountServiceListener::constructor(
    /* [in] */ Handle32 jVM,
    /* [in] */ Handle32 jInstance)
{
    mJVM = (JavaVM*)jVM;
    mJInstance = (jobject)jInstance;
    return NOERROR;
}

ECode CMountServiceListener::OnUsbMassStorageConnectionChanged(
    /* [in] */ Boolean connected)
{
    LOGGERD(TAG, String("CMountServiceListener E_NOT_IMPLEMENTED Line:%d"), __LINE__);
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CMountServiceListener::OnStorageStateChanged(
    /* [in] */ const String& path,
    /* [in] */ const String& oldState,
    /* [in] */ const String& newState)
{
    LOGGERD(TAG, String("CMountServiceListener E_NOT_IMPLEMENTED Line:%d"), __LINE__);
    assert(0);
    return E_NOT_IMPLEMENTED;
}

}
}
}

