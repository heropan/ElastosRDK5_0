#include "elastos/droid/server/input/InputApplicationHandle.h"
#include <utils/threads.h>

static android::Mutex gHandleMutex;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Input {

CAR_INTERFACE_IMPL(InputApplicationHandle, Object, IInputApplicationHandle);

InputApplicationHandle::InputApplicationHandle(
    /* [in] */ IObject* appWindowToken)
    : mAppWindowToken(appWindowToken)
    , mDispatchingTimeoutNanos(0)
    , mNative(0)
{}

InputApplicationHandle::~InputApplicationHandle()
{
    //try {
    NativeDispose();
    //} finally {
    //    super.finalize();
    //}
}

void InputApplicationHandle::NativeDispose()
{
/*
static void android_server_InputApplicationHandle_nativeDispose(JNIEnv* env, jobject obj) {
    AutoMutex _l(gHandleMutex);

    jlong ptr = env->GetLongField(obj, gInputApplicationHandleClassInfo.ptr);
    if (ptr) {
        env->SetLongField(obj, gInputApplicationHandleClassInfo.ptr, 0);

        NativeInputApplicationHandle* handle = reinterpret_cast<NativeInputApplicationHandle*>(ptr);
        handle->decStrong((void*)android_server_InputApplicationHandle_getHandle);
    }
}
    if (ptr != NULL) {
        NativeInputApplicationHandle* handle = reinterpret_cast<NativeInputApplicationHandle*>(ptr);
        handle->decStrong((void*)android_server_InputApplicationHandle_getHandle);
        ptr = NULL;
    }
*/
    android::AutoMutex _l(gHandleMutex);

    if (mNative != (Int64)NULL) {
        mNative->decStrong(this);
    }
}

android::sp<android::InputApplicationHandle> InputApplicationHandle::GetHandle(
    /* [in] */ InputApplicationHandle* inputApplicationHandleObj)
{
    if (inputApplicationHandleObj == NULL) {
        return NULL;
    }

    android::AutoMutex _l(gHandleMutex);

    NativeInputApplicationHandle* handle = inputApplicationHandleObj->mNative;
    if (handle == NULL) {
        handle = new NativeInputApplicationHandle(inputApplicationHandleObj);
        handle->incStrong(inputApplicationHandleObj);
        inputApplicationHandleObj->mNative = handle;
    }
    return handle;
}

} // Input
} // Server
} // Droid
} // Elastos
