#include "elastos/droid/server/input/InputWindowHandle.h"

namespace Elastos {
namespace Droid {
namespace Server {
namespace Input {

CAR_INTERFACE_IMPL(InputWindowHandle, Object, IInputWindowHandle);

InputWindowHandle::InputWindowHandle()
{}

InputWindowHandle::~InputWindowHandle()
{
    //try {
        NativeDispose();
    //} finally {
    //    super.finalize();
    //}
}

ECode InputWindowHandle::constructor()
{
    return NOERROR;
}

ECode InputWindowHandle::constructor(
    /* [in] */ IInputApplicationHandle* inputApplicationHandle,
    /* [in] */ IInterface* windowState,
    /* [in] */ Int32 displayId)
{
    mInputApplicationHandle = inputApplicationHandle;
    mWindowState = windowState;
    mDisplayId = displayId;

    return NOERROR;
}

void InputWindowHandle::NativeDispose()
{
/*
static void android_server_InputWindowHandle_nativeDispose(JNIEnv* env, jobject obj) {
    AutoMutex _l(gHandleMutex);

    jlong ptr = env->GetLongField(obj, gInputWindowHandleClassInfo.ptr);
    if (ptr) {
        env->SetLongField(obj, gInputWindowHandleClassInfo.ptr, 0);

        NativeInputWindowHandle* handle = reinterpret_cast<NativeInputWindowHandle*>(ptr);
        handle->decStrong((void*)android_server_InputWindowHandle_getHandle);
    }
}
*/
}

} // Input
} // Server
} // Droid
} // Elastos
