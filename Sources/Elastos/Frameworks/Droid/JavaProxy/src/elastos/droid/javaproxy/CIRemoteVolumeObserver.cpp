
#include "elastos/droid/javaproxy/CIRemoteVolumeObserver.h"
#include "elastos/droid/javaproxy/Util.h"

namespace Elastos {
namespace Droid {
namespace JavaProxy {

const String CIRemoteVolumeObserver::TAG("CIRemoteVolumeObserver");

CAR_INTERFACE_IMPL_2(CApplicationThreadNative, Object, IApplicationThread, IBinder)

CAR_OBJECT_IMPL(CApplicationThreadNative)

CIRemoteVolumeObserver::~CIRemoteVolumeObserver()
{
    JNIEnv* env;
    mJVM->AttachCurrentThread(&env, NULL);
    env->DeleteGlobalRef(mJInstance);
}

ECode CIRemoteVolumeObserver::constructor(
    /* [in] */ Handle32 jVM,
    /* [in] */ Handle32 jInstance)
{
    mJVM = (JavaVM*)jVM;
    mJInstance = (jobject)jInstance;
    return NOERROR;
}

ECode CIRemoteVolumeObserver::DispatchRemoteVolumeUpdate(
    /* [in] */ Int32 direction,
    /* [in] */ Int32 value)
{
    JNIEnv* env;
    mJVM->AttachCurrentThread(&env, NULL);

    jclass c = env->FindClass("android/media/IRemoteVolumeObserver");
    Util::CheckErrorAndLog(env, TAG, "Fail FindClass: IRemoteVolumeObserver %d", __LINE__);

    jmethodID m = env->GetMethodID(c, "dispatchRemoteVolumeUpdate", "(II)V");
    Util::CheckErrorAndLog(env, TAG, "GetMethodID: dispatchRemoteVolumeUpdate %d", __LINE__);

    env->CallVoidMethod(mJInstance, m, direction, value);
    Util::CheckErrorAndLog(env, TAG, "CallVoidMethod: dispatchRemoteVolumeUpdate %d", __LINE__);

    env->DeleteLocalRef(c);
    return NOERROR;
}

ECode CIRemoteVolumeObserver::ToString(
    /* [out] */ String* str)
{
    JNIEnv* env;
    mJVM->AttachCurrentThread(&env, NULL);
    *str = Util::GetJavaToStringResult(env, mJInstance);
    return NOERROR;
}

}
}
}

